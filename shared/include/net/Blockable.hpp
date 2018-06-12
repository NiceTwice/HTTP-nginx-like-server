//
// Blockable.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Fri Jan 20 23:35:09 2017 Adrien Vasseur
// Last update Fri Jan 20 23:35:09 2017 Adrien Vasseur
//

/*!
 * \file Blockable.hpp
 */

#pragma   once

/*! \class Blockable
 *  \brief Socket implementation mother class. It serve to block/unblock socket.
 */

class     Blockable
{
public:
  /*!
   *  \brief Blockable constructor
   */
  Blockable();

  /*!
   *  \brief Blockable destructor
   */
  ~Blockable();

  /*!
   *  \brief Permit to update his internal handler socket
   *  \param fd(int) new socket handler.
   */
  void    setHandler(int);

  /*!
   *  \brief Get his actually internal socket handler.
   *  \return fd(int) actually socket handler.
   */
  int     getHandler() const;

  /*!
   *  \brief Set the socket blocking mode
   *  \param value(bool) true to block the socket, false otherwise.
   */
  void    setBlocking(bool);

  /*!
   *  \brief Get his actually blocking mode.
   *  \return true if the socket is in blocked mode, false otherwise.
   */
  bool    isBlocking() const;

  /*!
   *  \brief Define the socket has connected. Util when we connect the socket for the first time.
   *  \param value(bool) true if the new socket is connected, false otherwise.
   */
  void    setConnected(bool);

  /*!
   *  \brief Does the socket is actually connected ?
   *  \return true if the socket is connected, false otherwise.
   */
  bool    isConnected() const;

protected:
  /*!
   *  \brief Internal socket handler
   */
  int     _socket;

  /*!
   *  \brief Internal socket connected status
   */
  bool    _connected;
};