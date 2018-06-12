//
// ISocket.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Fri Jan 20 23:27:33 2017 Adrien Vasseur
// Last update Fri Jan 20 23:27:33 2017 Adrien Vasseur
//

/*!
 * \file ISocket.hpp
 */

#pragma   once

#include  <string>
#include  <cstdint>

#include  "net/Buffer.hpp"

/*! \interface ISocket
 *  \brief Interface for socket implementations
 */

class     ISocket
{
public:
  /*!
   *  \brief ISocket destructor
   */
  virtual ~ISocket() {}

  /*!
   *  \brief Connect the socket to a server
   *  \param ip(const std::string &) the remote ip
   *  \param port(std::uint16_t) the remote port
   *  \return true on success, false otherwise
   *  \warning This method is not implemented in the ZIA project
   */
  virtual bool  connect(const std::string &, std::uint16_t) = 0;

  /*!
   *  \brief Send data to the peer
   *  \param buffer(Buffer &) read bytes from the buffer and remove them when they are sent
   *  \return true on success, false otherwise
   */
  virtual bool  send(Buffer &) = 0;

  /*!
   *  \brief Received data from the peer
   *  \param buffer(Buffer &) write bytes to the buffer
   *  \return true on success, false otherwise
   */
  virtual bool  recv(Buffer &) = 0;

  /*!
   *  \brief Disconnect the socket properly
   */
  virtual void  disconnect() = 0;

  /*!
   *  \brief Method to get the number of bytes waiting to be read in the socket
   *  \return size(std::uint64_t) return the number of bytes available on the socket handler
   */
  virtual std::uint64_t       bytesAvailable() const = 0;

  /*!
  *  \brief Get the local ip address
  *  \return ip(const std::string) return the local ip address
  */
  virtual const std::string   getAddress() const = 0;

  /*!
  *  \brief Get the local port
  *  \return port(std::uint16_t) return the local socket port
  */
  virtual std::uint16_t    getPort() const = 0;

  /*!
  *  \brief Get the peer ip address
  *  \return ip(const std::string) return the peer ip address
  */
  virtual const std::string   getRemoteAddress() const = 0;

  /*!
  *  \brief Get the peer port
  *  \return port(std::uint16_t) return the peer socket port
  */
  virtual std::uint16_t    getRemotePort() const = 0;
};