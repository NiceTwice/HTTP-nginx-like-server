//
// Core.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sat Jan 21 19:39:25 2017 Adrien Vasseur
// Last update Sat Jan 21 19:39:25 2017 Adrien Vasseur
//

#include    <chrono>
#include    <iostream>
#include    "loader/Pipeline.hpp"
#include    "thread/Clock.hpp"
#include    "utils/ParserUtils.hpp"
#include    "Core.hpp"

std::vector<Worker *>  Core::_workers;

Core::Core()
{
}

Core::~Core()
{
  for (std::vector<Worker *>::iterator ite = Core::_workers.begin(); ite != Core::_workers.end(); ite++)
    delete (*ite);
  for (std::vector<TcpListener *>::iterator ite = this->_servers.begin(); ite != this->_servers.end(); ite++)
    delete (*ite);
  Core::_workers.clear();
  this->_servers.clear();
}

bool        Core::listenAllPorts()
{
  const std::string *conf = this->_config.getConf("server.port");
  std::vector<std::string>  ports;

  if (conf == NULL)
    return (false);
  ParserUtils::split(ports, *conf, ',');
  if (ports.size() <= 0)
    return (false);
  ParserUtils::parsePorts(this->_listeningPorts, ports);
  if (this->_listeningPorts.size() <= 0)
    return (false);
  for (auto ite = this->_listeningPorts.begin(); ite != this->_listeningPorts.end(); ite++)
    this->listenPort(*ite);
  return (this->_servers.size() > 0);
}

bool        Core::listenPort(std::uint16_t port)
{
  TcpListener *listener;

  for (auto ite = this->_servers.begin(); ite != this->_servers.end(); ite++)
  {
    if ((*ite)->getPort() == port)
      return (true);
  }
  listener = new TcpListener;
  if (listener->listen(port))
  {
    std::cout << "[INFO] Listening on port " << listener->getPort() << "..." << std::endl;
    listener->setBlocking(false);
    this->_servers.push_back(listener);
    return (true);
  }
  delete listener;
  return (false);
}

bool        Core::init()
{
  std::uint32_t threads = Threaded::getHardwareThreads();

  if (!this->_config.load())
    return (false);
  if (!this->listenAllPorts())
    return (false);
  std::cout << "[INFO] Starting " << threads << " workers..." << std::endl;
  for (std::uint32_t i = 0; i < threads; i++)
  {
    apouche::IHttpConf  *newConf = new HttpConf(this->_config.getAllConf());
    Pipeline            *pipeline = new Pipeline(newConf);
    Worker              *worker = new Worker(i, this->_servers);

    if (!pipeline->init())
      return (false);
    worker->setNextPipeline(pipeline);
    if (worker->init())
    {
      worker->start();
      Core::_workers.push_back(worker);
    }
    else
    {
      std::cerr << "[ERROR] Cannot initialize worker" << std::endl;
      return (false);
    }
  }
  std::cout << "[INFO] Server started!" << std::endl;
  return (true);
}

void        Core::run()
{
  std::uint64_t lastTime = Clock::getCurrentTime();
  bool      running = true;

  while (running)
  {
    size_t  stopped = 0;

    for (std::vector<Worker *>::iterator ite = Core::_workers.begin(); ite != Core::_workers.end(); ite++)
      if (!((*ite)->isRunning()))
        stopped++;
    if (stopped == Core::_workers.size())
      running = false;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (Clock::getDiffTime(lastTime) > 5000)
    {
      if (this->_config.update())
      {
        for (std::vector<Worker *>::iterator ite = Core::_workers.begin(); ite != Core::_workers.end(); ite++)
        {
          apouche::IHttpConf  *newConf = new HttpConf(this->_config.getAllConf());
          Pipeline            *pipeline = new Pipeline(newConf);

          if (!pipeline->init())
          {
            this->stop();
            return;
          }
          (*ite)->setNextPipeline(pipeline);
        }
        std::cout << "[INFO] Configuration updated!" << std::endl;
      }
      lastTime = Clock::getCurrentTime();
    }
  }
  for (std::vector<Worker *>::iterator ite = Core::_workers.begin(); ite != Core::_workers.end(); ite++)
      (*ite)->join();
    
}

void        Core::stop()
{
  std::cout << "[INFO] Stopping server..." << std::endl;
  for (std::vector<Worker *>::iterator ite = Core::_workers.begin(); ite != Core::_workers.end(); ite++)
    (*ite)->stop();
}