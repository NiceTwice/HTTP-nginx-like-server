//
// TcpSocket.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Fri Jan 20 18:29:33 2017 Adrien Vasseur
// Last update Fri Jan 20 18:29:33 2017 Adrien Vasseur
//

/*!
 * \file TcpSocket.hpp
 */

#pragma     once

#include    <cstdint>
#include    <string>

#include    "net/ISocket.hpp"
#include    "net/Blockable.hpp"

/*! \class TcpSocket
 *  \brief TcpSocket usable implementation
 */

class       TcpSocket : public ISocket, public Blockable
{
public:
  /*!
   *  \brief TcpSocket constructor
   */
    TcpSocket();

  /*!
   *  \brief TcpSocket destructor
   */
    ~TcpSocket();

  /*!
   *  \brief Connect the socket to a server
   *  \param ip(const std::string &) the remote ip
   *  \param port(std::uint16_t) the remote port
   *  \return true on success, false otherwise
   *  \warning This method is not implemented in the ZIA project
   */
    bool            connect(const std::string &, std::uint16_t);

  /*!
   *  \brief Send data to the peer
   *  \param buffer(Buffer &) read bytes from the buffer and remove them when they are sent
   *  \return true on success, false otherwise
   */
    bool            send(Buffer &);

  /*!
   *  \brief Received data from the peer
   *  \param buffer(Buffer &) write bytes to the buffer
   *  \return true on success, false otherwise
   */
    bool            recv(Buffer &);

  /*!
   *  \brief Disconnect the socket properly
   */
    void            disconnect();

  /*!
   *  \brief Method to get the number of bytes waiting to be read in the socket
   *  \return size(std::uint64_t) return the number of bytes available on the socket handler
   */
    std::uint64_t   bytesAvailable() const;

  /*!
  *  \brief Get the local ip address
  *  \return ip(const std::string) return the local ip address
  */
    const std::string   getAddress() const;

  /*!
  *  \brief Get the local port
  *  \return port(std::uint16_t) return the local socket port
  */
    std::uint16_t    getPort() const;

  /*!
  *  \brief Get the peer ip address
  *  \return ip(const std::string) return the peer ip address
  */
    const std::string   getRemoteAddress() const;

  /*!
  *  \brief Get the peer port
  *  \return port(std::uint16_t) return the peer socket port
  */
    std::uint16_t    getRemotePort() const;
};