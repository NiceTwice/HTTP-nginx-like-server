//
// IListener.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Fri Jan 20 23:24:14 2017 Adrien Vasseur
// Last update Fri Jan 20 23:24:14 2017 Adrien Vasseur
//

/*!
 * \file IListener.hpp
 */

#pragma   once

#include  <cstdint>
#include  "net/ISocket.hpp"

/*! \interface IListener
 *  \brief Interface for all listening sockets
 */

class     IListener
{
public:
  /*!
   *  \brief IListener destructor
   */
  virtual ~IListener() {}

  /*!
   *  \brief Listen on a specified port
   *  \param port(std::uint16_t) is the listening port
   *  \return true on success, false otherwise
   */
  virtual bool  listen(std::uint16_t) = 0;

  /*!
   *  \brief Accept a new connected client
   *  \param socket(ISocket &) is the new connected client container
   *  \return true on success, false otherwise
   */
  virtual bool  accept(ISocket &) = 0;

  /*!
   *  \brief To stop the listening socket
   */
  virtual void  close() = 0;

  /*!
   *  \brief Get the actual listening port
   *  \return std::uint16_t actually listening port
   */
  virtual std::uint16_t   getPort() const = 0;
};