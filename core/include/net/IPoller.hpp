//
// IPoller.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sun Jan 22 21:04:54 2017 Adrien Vasseur
// Last update Sun Jan 22 21:04:54 2017 Adrien Vasseur
//

/*!
 * \file IPoller.hpp
 */

#pragma     once

#include    "net/NetClient.hpp"
#include    "net/TcpListener.hpp"

/*!
 *  \def Maximum events process at once
 */
#define     MAX_EVENTS  128

class       Worker;

/*!
 *  \brief Pointer on method to each event callback
 */
typedef bool  (Worker::*WorkerEvent_ptr)(void *);

/*!
 *  \namespace Poll
 *  \brief All polling utils enum
 */
namespace   Poll
{
  /*!
   *  \enum Type
   *  \brief Does the polling method is in read or write mode ?
   */
  enum      Type
  {
    READ,
    WRITE
  };

  /*!
   *  \enum Source
   *  \brief Does the polling is working on a listening socket or a client ?
   */
  enum      Source
  {
    LISTENER,
    CLIENT
  };
}

/*! \interface IPoller
 *  \brief Interface for all polling implementations
 */

class       IPoller
{
public:
  /*!
   * \struct Event
   * \brief A event stocked in the poller implementation class
   */
  struct    Event
  {
    /*!
     * \brief Client emitting the event
     */
    void     *client;

    /*!
     * \brief Client callback
     */
    WorkerEvent_ptr event;

    /*!
     * \brief Does the client is a socket or a listening socket ?
     */
    Poll::Source    source;

    Poll::Type      type;
  };

public:
  /*!
   *  \brief IPoller destructor
   */
  virtual   ~IPoller() {}

  /*!
   *  \brief Intialize the poller
   *  \return true on success, false otherwise
   */
  virtual bool  init() = 0;

  /*!
   *  \brief Add a connected client socket to the poller
   *  \param client(NetClient *) is the newly added client
   *  \param callback(WorkerEvent_ptr) is the client callback
   *  \param type(Poll::Type) is the client type
   */
  virtual void  add(NetClient *, WorkerEvent_ptr, Poll::Type) = 0;

  /*!
   *  \brief Add a listener client socket to the poller
   *  \param listener(TcpListener *) is the newly added listener
   *  \param callback(WorkerEvent_ptr) is the listener callback
   *  \param type(Poll::Type) is the client type
   */
  virtual void  add(TcpListener *, WorkerEvent_ptr, Poll::Type) = 0;

  /*!
   *  \brief Remove a connected client socket from the poller
   *  \param client(NetClient *) is the client to remove
   *  \param force(bool), if true the socket is also remove from the internal poller handler
   */
  virtual void  remove(NetClient *, bool) = 0;

  /*!
   *  \brief Remove a listening client socket from the poller
   *  \param listener(Listener *) is the listener to remove
   *  \param force(bool), if true the socket is also remove from the internal poller handler
   */
  virtual void  remove(TcpListener *, bool) = 0;

  /*!
   *  \brief Received a new polling event
   *  \param event(IPoller::Event * &) is the filled event
   *  \return false if no events has been received
   */
  virtual bool  pollEvent(IPoller::Event* &) = 0;

  /*!
   *  \brief Get number of clients in the poll
   *  \return size(std::uint64_t) number of clients actually in the poll
   */
  virtual std::uint64_t   getSize() const = 0;
};