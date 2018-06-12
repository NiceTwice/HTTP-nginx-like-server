//
// Poller.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sun Jan 22 21:03:08 2017 Adrien Vasseur
// Last update Sun Jan 22 21:03:08 2017 Adrien Vasseur
//

#ifndef _WIN32
#include    <unistd.h>
#else
#include    <winsock2.h>
#include    <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#endif
#include    <iostream>
#include    <cerrno>
#include    <cstring>
#include    "net/Poller.hpp"

Poller::Poller()
{
#ifndef _WIN32
  this->_id = -1;
#else
  this->_id = NULL;
  epoll__initialize();
#endif
  this->_events = NULL;
  this->_ite = 0;
  this->_nbrEvents = 0;
  this->_size = 0;
}

Poller::~Poller()
{
#ifndef _WIN32
  if (this->_id != -1)
    ::close(this->_id);
#else
  if (this->_id)
    epoll_close(this->_id);
#endif
  if (this->_events)
    delete[] this->_events;
  for (std::map<int, IPoller::Event *>::iterator ite = this->_entries.begin(); ite != this->_entries.end(); ite++)
  {
    if (ite->second->source == Poll::CLIENT && ite->second->type == Poll::READ)
      delete (NetClient *) (ite->second->client);
    delete ite->second;
  }
}

bool        Poller::init()
{
#ifndef _WIN32
  if (this->_id != -1)
    close(this->_id);
  this->_events = new struct epoll_event[MAX_EVENTS];
  return ((this->_id = epoll_create1(0)) != -1);
#else
  if (this->_id)
    CloseHandle(this->_id);
  return ((this->_id = epoll_create()) != NULL);
#endif
}

void        Poller::add(NetClient *client, WorkerEvent_ptr ptr, Poll::Type type)
{
  IPoller::Event      *data = new IPoller::Event;
  struct epoll_event  event;

  data->client = client;
  data->event = ptr;
  data->source = Poll::CLIENT;
  data->type = type;
  event.data.ptr = data;
  event.events = (type == Poll::READ ? EPOLLIN : EPOLLOUT);
  if (epoll_ctl(this->_id, EPOLL_CTL_ADD, client->getConnection().getNativeHandle().getHandler(), &event) == -1)
  {
    if (errno != EEXIST)
      std::cerr << "[ERROR] Cannot add fd in epoll: " << std::strerror(errno) << std::endl;
    delete data;
  }
  else
  {
    this->_entries[client->getConnection().getNativeHandle().getHandler()] = data;
    this->_size++;
  }
}

void        Poller::add(TcpListener *client, WorkerEvent_ptr ptr, Poll::Type type)
{
  IPoller::Event      *data = new IPoller::Event;
  struct epoll_event  event;

  data->client = client;
  data->event = ptr;
  data->source = Poll::LISTENER;
  data->type = type;
  event.data.ptr = data;
  event.events = (type == Poll::READ ? EPOLLIN : EPOLLOUT);
  if (epoll_ctl(this->_id, EPOLL_CTL_ADD, client->getHandler(), &event) == -1)
  {
    if (errno != EEXIST)
      std::cerr << "[ERROR] Cannot add fd in epoll: " << std::strerror(errno) << std::endl;
    delete data;
  }
  else
  {
    this->_entries[client->getHandler()] = data;
    this->_size++;
  }
}

void        Poller::remove(NetClient *client, bool force)
{
  std::map<int, IPoller::Event *>::iterator ite;

  if ((ite = this->_entries.find(client->getConnection().getNativeHandle().getHandler())) != this->_entries.end())
  {
    delete ite->second;
    this->_entries.erase(ite);
    this->_size--;
  }
  if (force && epoll_ctl(this->_id, EPOLL_CTL_DEL, client->getConnection().getNativeHandle().getHandler(), (epoll_event *) 1) == -1)
    std::cerr << "[ERROR] Cannot remove fd from epoll: " << std::strerror(errno) << std::endl;
}

void        Poller::remove(TcpListener *client, bool force)
{
  std::map<int, IPoller::Event *>::iterator ite;

  if ((ite = this->_entries.find(client->getHandler())) != this->_entries.end())
  {
    delete ite->second;
    this->_entries.erase(ite);
    this->_size--;
  }
  if (force && epoll_ctl(this->_id, EPOLL_CTL_DEL, client->getHandler(), (epoll_event *) 1) == -1)
    std::cerr << "[ERROR] Cannot remove fd from epoll: " << std::strerror(errno) << std::endl;
}

bool        Poller::pollEvent(IPoller::Event* &event)
{
#ifndef _WIN32
  if (this->_id == 1 || this->_events == NULL)
    return (false);
#else
  if (((epoll_data_t *)this->_id)->fd == 1 || this->_events == NULL)
    return (false);
#endif
  if (this->_nbrEvents != 0 && this->_ite >= this->_nbrEvents)
  {
    this->_nbrEvents = 0;
    return (false);
  }
  else if (this->_nbrEvents == 0)
  {
    this->_ite = 0;
    this->_nbrEvents = epoll_wait(this->_id, this->_events, MAX_EVENTS, 20);
  }
  if (this->_ite < this->_nbrEvents)
  {
    event = (IPoller::Event *) this->_events[this->_ite].data.ptr;
    this->_ite++;
    return (true);
  }
  return (false);
}

std::uint64_t Poller::getSize() const
{
  return (this->_size);
}