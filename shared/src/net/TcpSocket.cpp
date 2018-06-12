//
// TcpSocket.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Fri Jan 20 18:54:52 2017 Adrien Vasseur
// Last update Fri Jan 20 18:54:52 2017 Adrien Vasseur
//

#include    <iostream>
#ifndef     _WIN32
#include    <unistd.h>
#include    <sys/socket.h>
#include    <sys/types.h>
#include    <sys/ioctl.h>
#include    <netinet/in.h>
#include    <arpa/inet.h>
#else
#include    <winsock2.h>
#include    <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#endif
#include    <cerrno>
#include    <cstring>
#include    "net/TcpSocket.hpp"

TcpSocket::TcpSocket()
{
}

TcpSocket::~TcpSocket()
{
  if (this->_connected)
    this->disconnect();
}

/**
 * WARNING :
 *              Not implemented
 */

bool        TcpSocket::connect(const std::string &, std::uint16_t)
{
  this->disconnect();
  return (false);
}

bool        TcpSocket::send(Buffer &buffer)
{
  Buffer::Data  output;

  output.size = 0;
  buffer >> &output;
  if (this->_connected && output.size > 0)
  {
#ifndef _WIN32
    ssize_t written;
    if ((written = ::send(this->_socket, output.data, output.size, MSG_NOSIGNAL)) <= 0)
      return (false);
    buffer.removeFront(written);
#else
    unsigned __int64 written;
    if ((written = ::send((SOCKET) socket, output.data, output.size, 0)))
      return (false);
    buffer.removeFront(written);
#endif
  }
  return (true);
}

bool        TcpSocket::recv(Buffer &buffer)
{
  std::uint64_t lenght = this->bytesAvailable();
  std::int64_t  ret;
  Buffer::Data  *input;

  if (this->_connected && lenght > 0)
  {
    std::cout << "receive " << lenght << " bytes" << std::endl;
    input = Buffer::createData(lenght);
#ifdef _WIN32
    if ((ret = ::recv((SOCKET) this->_socket, input->data, lenght, 0)) <= 0)
#else
    if ((ret = ::recv(this->_socket, input->data, lenght, 0)) <= 0)
#endif
    {
      this->disconnect();
      return (false);
    }
    input->size = (std::uint64_t) ret;
    buffer << input;
    return (true);
  }
  return (false);
}

void        TcpSocket::disconnect()
{
#ifdef _WIN32
  if (this->_connected)
    closesocket(this->_socket);
#else
  if (this->_connected)
    ::close(this->_socket);
#endif
}

/*
 * WARNING:
 *            Use ioctlsocket instead of ioctl on Windows systems
 */

std::uint64_t   TcpSocket::bytesAvailable() const
{
  std::uint64_t bytes = 0;
#ifndef _WIN32
  if (!this->_connected || ::ioctl(this->_socket, FIONREAD, &bytes) == -1)
#else
  if (!this->_connected || ioctlsocket(this->_socket, FIONREAD, (u_long *) &bytes) == -1)
#endif
    return (0);
  return (bytes);
}

const std::string   TcpSocket::getAddress() const
{
  struct sockaddr_in  addr;
  socklen_t           len;
  char                *ret;

  if (!this->_connected)
    return (0);
  len = sizeof(addr);
  if (::getsockname(this->_socket, (struct sockaddr *) &addr, &len) == -1)
  {
    int error = errno;
    std::cerr << "[ERROR] Cannot get listening port: " << std::strerror(error) << std::endl;
    return ("");
  }
  if ((ret = inet_ntoa(addr.sin_addr)) != NULL)
    return (ret);
  return ("");
}

std::uint16_t  TcpSocket::getPort() const
{
  struct sockaddr_in  addr;
  socklen_t           len;

  if (!this->_connected)
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

const std::string   TcpSocket::getRemoteAddress() const
{
  struct sockaddr_in  addr;
  socklen_t           len;
  char                *ret;

  if (!this->_connected)
    return (0);
  len = sizeof(addr);
  if (::getpeername(this->_socket, (struct sockaddr *) &addr, &len) == -1)
  {
    int error = errno;
    std::cerr << "[ERROR] Cannot get listening port: " << std::strerror(error) << std::endl;
    return ("");
  }
  if ((ret = inet_ntoa(addr.sin_addr)) != NULL)
    return (ret);
  return ("");
}

std::uint16_t  TcpSocket::getRemotePort() const
{
  struct sockaddr_in  addr;
  socklen_t           len;

  if (!this->_connected)
    return (0);
  len = sizeof(addr);
  if (::getpeername(this->_socket, (struct sockaddr *) &addr, &len) == -1)
  {
    int error = errno;
    std::cerr << "[ERROR] Cannot get listening port: " << std::strerror(error) << std::endl;
    return (0);
  }
  return (ntohs(addr.sin_port));
}
