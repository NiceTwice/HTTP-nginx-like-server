//
// NetClient.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sun Jan 22 20:57:02 2017 Adrien Vasseur
// Last update Sun Jan 22 20:57:02 2017 Adrien Vasseur
//

/*!
 * \file NetClient.hpp
 */

#pragma   once

#include <api/Network/NetworkStatus.hpp>
#include  "api/Network/ZiaConnection.hpp"
#include  "net/TcpSocket.hpp"
#include  "net/Buffer.hpp"
#include  "api/HttpConf/IHttpConf.hpp"
#include  "api/EventHandler/EventHandler.hpp"

/*! \class NetClient
 *  \brief A NetClient is a complete connected client. It's composed by his socket and his buffers.
 */

class     NetClient
{
public:
  /*!
   *  \brief NetClient constructor
   */
  NetClient(apouche::EventHandler *, apouche::IHttpConf *);

  /*!
   *  \brief NetClient destructor
   */
  ~NetClient();

  /*!
   *  \brief Process permit to read his socket and process his input internal data
   *  \return If socket has been closed, return false. Return true otherwise.
   */
  bool        process();

  /*!
   *  \brief Flush permit to write all his internal output data to his socket
   *  \return If socket has been closed, return false. Return true otherwise.
   */
  bool        flush();

  apouche::IZiaConnection   &getConnection();

  /*!
   *  \brief Does the NetClient is waiting to write data to his socket ?
   *  \return Return true if his internal output buffer is not empty, false otherwise.
   */
  bool        hasToWrite() const;

  bool        onConnection();

private:
  ZiaConnection _connection;
  NetworkStatus _network;

  apouche::ReturnState  executePipeline();

  apouche::EventHandler *_pipeline;
  apouche::IHttpConf    *_conf;
  apouche::IHttpRequest *_httpRequest;
  apouche::IHttpResponse *_httpResponse;
};