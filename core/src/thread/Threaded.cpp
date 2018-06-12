//
// Threaded.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sat Jan 21 18:46:13 2017 Adrien Vasseur
// Last update Sat Jan 21 18:46:13 2017 Adrien Vasseur
//

#include    "thread/Threaded.hpp"

Threaded::Threaded()
{
  this->_thread = NULL;
  this->_running = false;
}

Threaded::~Threaded()
{
  if (this->_thread.load())
    delete this->_thread.load();
}

void        Threaded::start()
{
  if (this->_thread.load())
    delete this->_thread.load();
  this->_thread = new std::thread(&Threaded::_internalRun, this);
  this->_running = true;
}

void        Threaded::stop()
{
  this->_running = false;
}

void        Threaded::join()
{
  std::thread *tmp;

  tmp = this->_thread.load();
  if (tmp)
    tmp->join();
}

bool        Threaded::isRunning() const
{
  return (this->_running.load());
}

void        Threaded::_internalRun()
{
  while (this->_running)
    this->run();
}

std::uint32_t Threaded::getHardwareThreads()
{
  std::uint32_t   threads;

  if ((threads = std::thread::hardware_concurrency()) > 0)
    return (threads);
  return (1);
}