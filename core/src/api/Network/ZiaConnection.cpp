//
// ZiaConnection.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Tue Jan 31 20:32:27 2017 Adrien Vasseur
// Last update Tue Jan 31 20:32:27 2017 Adrien Vasseur
//

#include <iostream>
#include <api/Event/Event.hpp>
#include <api/HttpRequest/HttpRequest.hh>
#include "api/Network/ZiaConnection.hpp"

ZiaConnection::ZiaConnection()
{
  this->_forceWrite = false;
  this->_data = NULL;
}

ZiaConnection::~ZiaConnection()
{
}

apouche::IZiaConnection::NativeHandle  &ZiaConnection::getNativeHandle()
{
  return (this->_handler);
}

int         ZiaConnection::ReceiveRequest()
{
  if (!this->_handler.recv(this->_input))
  {
    this->_handler.disconnect();
    return (apouche::FATAL_ERROR);
  }
  return (apouche::SUCCESS);
}

int         ZiaConnection::SendResponse(apouche::IHttpResponse *response)
{
  if (response == NULL)
  {
    if (!this->_handler.send(this->_output) || !this->_output.hasData())
    {
      this->_handler.disconnect();
      return (apouche::FATAL_ERROR);
    }
  }
  else
  {
    Buffer::Data  *data = HttpParser::toData(response);

    this->_output << data;
  }
  return ((int) apouche::SUCCESS);
}

std::vector<apouche::IHttpRequest *> const  &ZiaConnection::getRequests() const
{
  return (this->_unknown);
}

apouche::IHttpRequest     *ZiaConnection::getNextRequest()
{
  Buffer::Data            data;

  this->_input.assemble();
  this->_input >> &data;
  apouche::IHttpRequest *request = new apouche::HttpRequest;
  if (data.size == 0 || !this->_httpParser.parseRequest(request, std::string(data.data, data.size))) {
    delete request;
    return (NULL);
  }
  return (request);
}

void        ZiaConnection::waitToWrite(bool writer)
{
  this->_forceWrite = writer;
}

bool        ZiaConnection::waitToWrite() const
{
  return (this->_forceWrite || this->_output.hasData());
}

Buffer      &ZiaConnection::getInput()
{
  return (this->_input);
}

Buffer      &ZiaConnection::getOutput()
{
  return (this->_output);
}

void        *ZiaConnection::getData() const
{
  return (this->_data);
}

void        ZiaConnection::setData(void *ptr)
{
  this->_data = ptr;
}