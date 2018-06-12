//
// TcpListener.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Fri Jan 20 18:56:20 2017 Adrien Vasseur
// Last update Fri Jan 20 18:56:20 2017 Adrien Vasseur
//

#include    <iostream>
#ifdef _WIN32
#include    <winsock2.h>
#include    <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include    <netinet/in.h>
#include    <netdb.h>
#include    <unistd.h>
#endif
#include    <cerrno>
#include    <cstring>
#include    <typeinfo>
#include    "net/TcpListener.hpp"
#include    "net/TcpSocket.hpp"

TcpListener::TcpListener()
{
}

TcpListener::~TcpListener()
{
  if (this->_socket != -1)
    this->close();
}

bool        TcpListener::listen(std::uint16_t port)
{
  struct protoent     *protocol;
  struct sockaddr_in  addr;

  if (!(protocol = ::getprotobyname("TCP")))
  {
    int error = errno;
    std::cerr << "[ERROR] Cannot find protocol TCP: " << std::strerror(error) << std::endl;
    return (false);
  }
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  if ((this->_socket = ::socket(AF_INET, SOCK_STREAM, protocol->p_proto)) == -1)
  {
    int error = errno;
    std::cerr << "[ERROR] Cannot create socket: " << std::strerror(error) << std::endl;
    return (false);
  }
  else if (::bind(this->_socket, (const struct sockaddr *) &addr, sizeof(addr)) == -1)
  {
    int error = errno;
    std::cerr << "[ERROR] Cannot bind port " << port << ": " << std::strerror(error) << std::endl;
    this->close();
    return (false);
  }
  else if (::listen(this->_socket, SOMAXCONN) == -1)
  {
    int error = errno;
    std::cerr << "[ERROR] Cannot listen socket: " << std::strerror(error) << std::endl;
    this->close();
    return (false);
  }
  return (true);
}

bool        TcpListener::accept(ISocket &client)
{
  struct sockaddr_in  addr;
  socklen_t           len;
  int                 socket;

  len = sizeof(addr);
  if ((socket = ::accept(this->_socket, (struct sockaddr *) &addr, &len)) == -1)
  {
    int error = errno;
    if (error != EWOULDBLOCK)
      std::cerr << "[ERROR] Cannot accept new client: " << std::strerror(error) << std::endl;
    return (false);
  }
  try
  {
    dynamic_cast<TcpSocket &>(client).setHandler(socket);
  } catch (std::bad_cast &e)
  {
    std::cerr << "[ERROR] Cannot accept new client: bad client" << std::endl;
    return (false);
  }
  return (true);
}

void        TcpListener::close()
{
  if (this->_socket != -1)
#ifdef _WIN32
    ::closesocket(this->_socket);
#else
  ::close(this->_socket);
#endif
  this->_socket = -1;
}

std::uint16_t  TcpListener::getPort() const
{
  struct sockaddr_in  addr;
  socklen_t           len;

  if (this->_socket == -1)
    return (0);
  len = sizeof(addr);
  if (::getsockname(this->_socket, (struct sockaddr *) &addr, &len) == -1)
  {
    int error = errno;
    std::cerr << "[ERROR] Cannot get listening port: " << std::strerror(error) << std::endl;
    return (0);
  }
  return (ntohs(addr.sin_port));
}