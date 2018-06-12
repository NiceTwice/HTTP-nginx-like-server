//
// SSLModule.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Wed Feb 01 13:09:18 2017 Adrien Vasseur
// Last update Wed Feb 01 13:09:18 2017 Adrien Vasseur
//

#pragma   once

#include  <openssl/bio.h>
#include  <openssl/ssl.h>
#include  <openssl/err.h>
#include  "api/Module/AModule.hpp"
#include  "SSLSocket.hpp"

class     SSLModule : public apouche::AModule
{
public:
  struct  Context
  {
    SSL_CTX *_ssl;
    BIO     *_bio;
  };

  SSLModule(apouche::Logger * = new apouche::Logger());
  ~SSLModule();

  void    initSSL();
  void    createCTX(const std::string &, const std::string &, std::uint16_t);
  void    freeSSL();
  void    freeCTX(SSL_CTX *, BIO *);
  Context *getContext(std::uint16_t);

  void    createContextOnPort(apouche::IHttpConf *, std::uint16_t);
  void    createContexts(apouche::IHttpConf *);

  void    registerEvents(apouche::EventHandler *);

  apouche::ReturnState  onIOEvent(apouche::IHttpConf *, apouche::INetworkStatus *);

private:
  apouche::ReturnState  onReadEvent(apouche::IHttpConf *, apouche::IZiaConnection *);
  apouche::ReturnState  onWriteEvent(apouche::IHttpConf *, apouche::IZiaConnection *);

  apouche::ReturnState  handleRead(apouche::IZiaConnection *, std::uint16_t);
  apouche::ReturnState  handleWrite(apouche::IZiaConnection *, std::uint16_t);

  std::map<std::uint16_t, Context *>  _contexts;
  bool                  _initialized;
};

extern "C" apouche::AModule *instantiate() {
  return new SSLModule();
}

extern "C" apouche::AModule *instantiateWithLogger(apouche::Logger *log){
  return new SSLModule(log);
}
