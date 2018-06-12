//
// NetClient.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sun Jan 22 21:00:02 2017 Adrien Vasseur
// Last update Sun Jan 22 21:00:02 2017 Adrien Vasseur
//

#include <iostream>
#include <api/HttpResponse/HttpResponse.hh>
#include "api/HttpBody/HttpBody.hh"
#include "api/HttpHeader/HttpHeader.hh"
#include "api/HttpRequest/HttpRequest.hh"
#include "parser/HttpParser.hh"
#include "net/NetClient.hpp"

NetClient::NetClient(apouche::EventHandler *pipeline, apouche::IHttpConf *conf) : _pipeline(pipeline), _conf(conf)
{
  this->_httpRequest = NULL;
  this->_httpResponse = new apouche::HttpResponse(new apouche::HttpHeader, new apouche::HttpBody, "HTTP/1.1");
}

NetClient::~NetClient()
{
  if (this->_httpRequest)
    delete this->_httpRequest;
  if (this->_httpResponse)
    delete this->_httpResponse;
}

apouche::ReturnState    NetClient::executePipeline()
{
  apouche::ReturnState  state = apouche::ReturnState::SUCCESS;

  while (state != apouche::ReturnState::FATAL_ERROR &&
         this->_pipeline->_beforeParsingRequest.callPollEvent(state, this->_conf));
  if (state != apouche::ReturnState::FATAL_ERROR)
  {
    try {
      if ((this->_httpRequest = this->_connection.getNextRequest()) == NULL)
        return (apouche::ERROR);
      this->_httpResponse->setStatus(apouche::OK);
    } catch (HttpParserException const& e) {
      std::cerr << "[ERROR] " << e.what() << std::endl;
      this->_httpResponse->setStatus(e.code());
    }
  }
  while (state != apouche::ReturnState::FATAL_ERROR &&
         this->_pipeline->_afterParsingRequest.callPollEvent(state, this->_httpRequest, this->_conf));
  while (state != apouche::ReturnState::FATAL_ERROR &&
         this->_pipeline->_requestReceived.callPollEvent(state, this->_httpRequest, this->_conf, &(this->_connection)));
  while (state != apouche::ReturnState::FATAL_ERROR &&
         this->_pipeline->_beforeCreateResponse.callPollEvent(state, this->_httpRequest, this->_conf));
  while (state != apouche::ReturnState::FATAL_ERROR &&
         this->_pipeline->_afterCreateResponse.callPollEvent(state, this->_httpRequest, this->_httpResponse, this->_conf));
  while (state != apouche::ReturnState::FATAL_ERROR &&
         this->_pipeline->_beforeSendResponse.callPollEvent(state, this->_httpResponse, this->_conf));
  while (state != apouche::ReturnState::FATAL_ERROR &&
         this->_pipeline->_afterSendResponse.callPollEvent(state, &(this->_connection)));
  return (state);
}

bool        NetClient::process()
{
  apouche::ReturnState  state = apouche::ReturnState::SUCCESS;

  this->_network.addConnection(&(this->_connection), apouche::INetworkStatus::READ);
  while (state != apouche::ReturnState::FATAL_ERROR &&
          this->_pipeline->_onNetworkIO.callPollEvent(state, this->_conf, &this->_network));
  this->_network.clear();
  if (state == apouche::ReturnState::SUCCESS)
  {
    if (this->_httpRequest)
      delete this->_httpRequest;
    this->_httpRequest = NULL;
    state = this->executePipeline();
    if (state == apouche::SUCCESS)
      this->_connection.SendResponse(this->_httpResponse);
  }
  return (state != apouche::ReturnState::FATAL_ERROR);
}

bool        NetClient::flush()
{
  apouche::ReturnState  state = apouche::ReturnState::SUCCESS;

  this->_network.addConnection(&(this->_connection), apouche::INetworkStatus::WRITE);
  while (state != apouche::ReturnState::FATAL_ERROR &&
         this->_pipeline->_onNetworkIO.callPollEvent(state, this->_conf, &this->_network));
  this->_network.clear();
  return (state != apouche::ReturnState::FATAL_ERROR);
}

bool        NetClient::onConnection()
{
  apouche::ReturnState  state = apouche::ReturnState::SUCCESS;

  while (state != apouche::ReturnState::FATAL_ERROR &&
          this->_pipeline->_afterConnect.callPollEvent(state, &(this->_connection), this->_conf));
  return (state != apouche::ReturnState::FATAL_ERROR);
}

apouche::IZiaConnection   &NetClient::getConnection()
{
  return (this->_connection);
}

bool        NetClient::hasToWrite() const
{
  return (this->_connection.waitToWrite());
}