//
// TcpListener.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Fri Jan 20 18:30:05 2017 Adrien Vasseur
// Last update Fri Jan 20 18:30:05 2017 Adrien Vasseur
//

/*!
 * \file TcpListener.hpp
 */

#pragma     once

#include    <cstdint>

#include    "net/IListener.hpp"
#include    "net/Blockable.hpp"

/*! \interface TcpListener
 *  \brief TcpListener usable implementation
 */

class       TcpListener : public IListener, public Blockable
{
public:
  /*!
   *  \brief TcpListener constructor
   */
    TcpListener();

  /*!
   *  \brief TcpListener destructor
   */
    ~TcpListener();

  /*!
   *  \brief Listen on a specified port
   *  \param port(std::uint16_t) is the listening port
   *  \return true on success, false otherwise
   */
    bool            listen(std::uint16_t);

  /*!
   *  \brief Accept a new connected client
   *  \param socket(ISocket &) is the new connected client container
   *  \return true on success, false otherwise
   */
    bool            accept(ISocket &);

  /*!
   *  \brief To stop the listening socket
   */
    void            close();

  /*!
   *  \brief Get the actual listening port
   *  \return std::uint16_t actually listening port
   */
    std::uint16_t   getPort() const;
};