//
// Poller.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sun Jan 22 20:54:55 2017 Adrien Vasseur
// Last update Sun Jan 22 20:54:55 2017 Adrien Vasseur
//

/*!
 * \file Poller.hpp
 */


#pragma   once

#ifndef ZIA_POLLER_HPP
#define ZIA_POLLER_HPP

#include  <map>
#ifndef   _WIN32
#include  <sys/epoll.h>
#else
#include <net/Epoll.hpp>
#endif

#include  "net/IPoller.hpp"

/*! \class Poller
 *  \brief Polling usable implementation
 */

class     Poller
{
public:
  /*!
   *  \brief Poller constructor
   */
  Poller();

  /*!
   *  \brief Poller destructor
   */
  ~Poller();

  /*!
   *  \brief Poller initializer
   *  \return true on success, false otherwise
   */
  bool    init();

  /*!
   *  \brief Add a connected client socket to the poller
   *  \param client(NetClient *) is the newly added client
   *  \param callback(WorkerEvent_ptr) is the client callback
   *  \param type(Poll::Type) is the client type
   */
  void    add(NetClient *, WorkerEvent_ptr, Poll::Type);

  /*!
   *  \brief Add a listener client socket to the poller
   *  \param listener(TcpListener *) is the newly added listener
   *  \param callback(WorkerEvent_ptr) is the listener callback
   *  \param type(Poll::Type) is the client type
   */
  void    add(TcpListener *, WorkerEvent_ptr, Poll::Type);

  /*!
   *  \brief Remove a connected client socket from the poller
   *  \param client(NetClient *) is the client to remove
   *  \param force(bool), if true the socket is also remove from the internal poller handler
   */
  void    remove(NetClient *, bool = false);

  /*!
   *  \brief Remove a listening client socket from the poller
   *  \param listener(Listener *) is the listener to remove
   *  \param force(bool), if true the socket is also remove from the internal poller handler
   */
  void    remove(TcpListener *, bool = false);

  /*!
   *  \brief Received a new polling event
   *  \param event(IPoller::Event * &) is the filled event
   *  \return false if no events has been received
   */
  bool    pollEvent(IPoller::Event* &);

  /*!
   *  \brief Get number of clients in the poll
   *  \return size(std::uint64_t) number of clients actually in the poll
   */
  std::uint64_t   getSize() const;

private:
  /*!
   * \brief Internal poller handler
   */
#ifndef _WIN32
  int     _id;
#else
  epoll_t  _id;
#endif

  /*!
   * \brief Internal poller returns events
   */
  struct epoll_event  *_events;

  /*!
   * \brief Internal poller events iterator
   */
  int     _ite;

  /*!
   * \brief Internal poller events number
   */
  int     _nbrEvents;

  /*!
   * \brief Internal poller size
   */
  std::uint64_t _size;

  /*!
   * \brief Internal poller events list
   */
  std::map<int, IPoller::Event *> _entries;
};

#endif /* ZIA_POLLER_HPP */