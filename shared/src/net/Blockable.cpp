//
// Blockable.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Fri Jan 20 23:42:32 2017 Adrien Vasseur
// Last update Fri Jan 20 23:42:32 2017 Adrien Vasseur
//

#include    <iostream>
#include    <cerrno>
#include    <cstring>
#ifndef _WIN32
#include    <fcntl.h>
#else
#include    <winsock2.h>
#include    <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#endif
#include    "net/Blockable.hpp"

Blockable::Blockable()
{
  this->_socket = -1;
  this->_connected = false;
}

Blockable::~Blockable()
{
}

void        Blockable::setHandler(int socket)
{
  this->_socket = socket;
}

int         Blockable::getHandler() const
{
  return (this->_socket);
}

void        Blockable::setBlocking(bool blocking)
{
  int       flags;

  if (this->_socket == -1)
    return ;
#ifndef _WIN32
  if ((flags = ::fcntl(this->_socket, F_GETFL, NULL)) == -1)
  {
    int error = errno;
    std::cerr << "[ERROR] Cannot get flags socket: " << std::strerror(error) << std::endl;
    return ;
  }
  if (!blocking)
    flags |= O_NONBLOCK;
  else
    flags &= ~O_NONBLOCK;
  if (::fcntl(this->_socket, F_SETFL, flags) == -1)
  {
    int error = errno;
    std::cerr << "[ERROR] Cannot set flags socket: " << std::strerror(error) << std::endl;
  }
#else
  u_long iMode = 0;
  if (blocking)
    iMode = 1;
  int iResult = ioctlsocket(this->_socket, FIONBIO, &iMode);
  if (iResult != NO_ERROR)
    std::cerr << "[ERROR] Cannot set flags socket, return code is " << iResult << std::endl;
#endif
}

bool        Blockable::isBlocking() const
{
  int     flags;

  if (this->_socket == -1)
    return (false);
#ifndef _WIN32
  if ((flags = ::fcntl(this->_socket, F_GETFL, NULL)) == -1)
  {
    int error = errno;
    std::cerr << "[ERROR] Cannot get flags socket: " << std::strerror(error) << std::endl;
    return (false);
  }
  return ((flags & O_NONBLOCK) == 0);
#else
  std::cerr << "[ERROR] Windows do not provide any way to get socket blocking flag, return false by default." << std::endl;
  return (false);
#endif
}

void        Blockable::setConnected(bool connected)
{
  this->_connected = connected;
}

bool        Blockable::isConnected() const
{
  return (this->_connected);
}