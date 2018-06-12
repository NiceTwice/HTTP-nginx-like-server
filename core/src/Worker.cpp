//
// Worker.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sat Jan 21 18:56:15 2017 Adrien Vasseur
// Last update Sat Jan 21 18:56:15 2017 Adrien Vasseur
//

#include  <iostream>
#include  "Worker.hpp"

Worker::Worker(int id, std::vector<TcpListener *> &listeners) : _id(id), _listeners(listeners), _pipeline(NULL), _conf(NULL)
{
  this->_nextPipeline = NULL;
  this->_lastPipeline = NULL;
}

Worker::~Worker()
{
  if (this->_nextPipeline.load() != NULL)
    delete this->_nextPipeline.load();
}

bool      Worker::init()
{
  if (this->_inPoll.init() && this->_outPoll.init())
  {
    for (auto ite = this->_listeners.begin(); ite != this->_listeners.end(); ite++)
      this->_inPoll.add(*ite, &Worker::onNewClient, Poll::READ);
    return (true);
  }
  return (false);
}

void      Worker::run()
{
  Pipeline        *pipeline = this->_nextPipeline.load();
  IPoller::Event  *event;

  if (pipeline == NULL)
    return;
  this->_conf = pipeline->getConfig();
  this->_pipeline = pipeline->getPipeline();
  while (this->_inPoll.pollEvent(event))
    (this->*(event->event))(event);
  while (this->_outPoll.getSize() > 0 && this->_outPoll.pollEvent(event))
    (this->*(event->event))(event);
  pipeline = this->_lastPipeline.exchange(NULL);
  if (pipeline != NULL)
    delete pipeline;
}

bool      Worker::onNewClient(void *ptr)
{
  IPoller::Event  *event = (IPoller::Event *) ptr;
  TcpListener *listener = (TcpListener *) event->client;
  NetClient   *newClient = new NetClient(this->_pipeline, this->_conf);

  if (listener->accept(newClient->getConnection().getNativeHandle()))
  {
    newClient->getConnection().getNativeHandle().setConnected(true);
    newClient->getConnection().getNativeHandle().setBlocking(false);
    this->_inPoll.add(newClient, &Worker::onClientRead, Poll::READ);
    std::cout << "[" << this->_id << "] New client connected: " << newClient->getConnection().getNativeHandle().getRemoteAddress()
              << ":" << newClient->getConnection().getNativeHandle().getRemotePort() << std::endl;
    if (!newClient->onConnection())
    {
      this->_inPoll.remove(newClient);
      delete newClient;
      std::cout << "[" << this->_id << "] Client disconnected" << std::endl;
      return (true);
    }
  }
  else
    delete newClient;
  return (true);
}

bool      Worker::onClientRead(void *ptr)
{
  IPoller::Event  *event = (IPoller::Event *) ptr;
  NetClient *client = (NetClient *) (event->client);

  if (!client->process())
  {
    this->_inPoll.remove(client);
    this->_outPoll.remove(client);
    delete client;
    std::cout << "[" << this->_id << "] Client disconnected" << std::endl;
    return (true);
  }
  if (client->hasToWrite())
    this->_outPoll.add(client, &Worker::onClientWrite, Poll::WRITE);
  return (true);
}

bool      Worker::onClientWrite(void *ptr)
{
  IPoller::Event  *event = (IPoller::Event *) ptr;
  NetClient *client = (NetClient *) (event->client);

  if (!client->flush())
  {
    this->_inPoll.remove(client);
    this->_outPoll.remove(client);
    delete client;
    std::cout << "[" << this->_id << "] Client disconnected" << std::endl;
    return (true);
  }
  if (!client->hasToWrite())
    this->_outPoll.remove(client, true);
  return (true);
}

void    Worker::setNextPipeline(Pipeline *next)
{
  this->_lastPipeline.store(this->_nextPipeline.exchange(next));
}