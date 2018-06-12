//
// Worker.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sat Jan 21 18:54:57 2017 Adrien Vasseur
// Last update Sat Jan 21 18:54:57 2017 Adrien Vasseur
//

/*!
 * \file Worker.hpp
 */

#pragma   once

#include  <vector>
#include <loader/Pipeline.hpp>
#include  "thread/Threaded.hpp"
#include  "net/TcpListener.hpp"
#include  "net/TcpSocket.hpp"
#include  "net/NetClient.hpp"
#include  "net/Poller.hpp"
#include  "api/HttpConf/IHttpConf.hpp"
#include  "api/EventHandler/EventHandler.hpp"

/*! \class Worker
 *  \brief Working threads : the Core has n workers, each workers is a independent thread listening on the same port
 */

class     Worker : public Threaded
{
public:
  /*!
   *  \brief ZIA Core constructor
   *  \param id(int) the unique worker id
   *  \param listener(TcpListener) the common TcpListener waiting for new client
   */
  Worker(int, std::vector<TcpListener *> &);

  /*!
   *  \brief Worker destructor
   */
  ~Worker();

  /*!
   *  \brief Worker initializer, initialize input and output Poller and set listener in the input Poller
   *  \return true on success, false otherwise
   */
  bool    init();

  void    setNextPipeline(Pipeline *);

protected:
  /*!
   *  \brief Worker internal run, is called by the parent Threaded class
   */
  void    run();

private:
  /*!
   *  \brief Worker internal unique id
   */
  int           _id;

  /*!
   *  \brief Common listener between all workers
   */
  std::vector<TcpListener *>  &_listeners;

  /*!
   *  \brief Input poller containing all readable files descriptors
   */
  Poller        _inPoll;

  /*!
   *  \brief Output poller containing all writeable files descriptors
   */
  Poller        _outPoll;

  /*!
   *  \brief Currently connected clients, saved as NetClient pointers
   */
  std::vector<NetClient *>  _clientList;

  /*!
   *  \brief Internal callback called when a new client is trying to connect
   *  \param event(void *) the event emitted
   *  \return true on success, false otherwise
   */
  bool          onNewClient(void *);

  /*!
   *  \brief Internal callback called when a client needs to read his socket
   *  \param event(void *) the event emitted
   *  \return true on success, false otherwise
   */
  bool          onClientRead(void *);

  /*!
   *  \brief Internal callback called when a client needs to write his socket
   *  \param event(void *) the event emitted
   *  \return true on success, false otherwise
   */
  bool          onClientWrite(void *);

  apouche::EventHandler *_pipeline;
  apouche::IHttpConf    *_conf;

  std::atomic<Pipeline *> _lastPipeline;
  std::atomic<Pipeline *> _nextPipeline;
};