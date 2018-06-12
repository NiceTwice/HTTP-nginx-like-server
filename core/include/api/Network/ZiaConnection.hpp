//
// ZiaConnection.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Tue Jan 31 20:14:08 2017 Adrien Vasseur
// Last update Tue Jan 31 20:14:08 2017 Adrien Vasseur
//

#pragma   once

#include  "api/Network/IZiaConnection.hpp"
#include  "api/HttpResponse/IHttpResponse.hh"
#include  "api/HttpRequest/IHttpRequest.hh"
#include  "parser/HttpParser.hh"

class     ZiaConnection : public apouche::IZiaConnection
{
public:
  ZiaConnection();
  ~ZiaConnection();

  NativeHandle  &getNativeHandle();
  int           ReceiveRequest();
  int           SendResponse(apouche::IHttpResponse *);
  std::vector<apouche::IHttpRequest *> const  &getRequests() const;
  apouche::IHttpRequest                       *getNextRequest();

  void          waitToWrite(bool);
  bool          waitToWrite() const;

  Buffer        &getInput();
  Buffer        &getOutput();

  void          *getData() const;
  void          setData(void *);

private:
  apouche::IZiaConnection::NativeHandle _handler;
  std::vector<apouche::IHttpRequest *>  _unknown;

  Buffer      _input;
  Buffer      _output;

  HttpParser  _httpParser;

  bool        _forceWrite;

  void        *_data;
};