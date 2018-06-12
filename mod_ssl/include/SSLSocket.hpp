//
// SSLSocket.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Wed Feb 01 14:47:51 2017 Adrien Vasseur
// Last update Wed Feb 01 14:47:51 2017 Adrien Vasseur
//

#pragma   once

#include  <openssl/bio.h>
#include  <openssl/ssl.h>
#include  <openssl/err.h>
#include <api/Network/IZiaConnection.hpp>
#include  "api/Event/Event.hpp"

class     SSLSocket
{
public:
  SSLSocket();
  ~SSLSocket();

  apouche::ReturnState  handshake(SSL_CTX *, BIO *, apouche::IZiaConnection *);
  apouche::ReturnState  readData(apouche::IZiaConnection *);
  apouche::ReturnState  writeData(apouche::IZiaConnection *);

  void    close();

  bool    isSslConnected();

private:
  SSL     *_ssl;
  bool    _sslConnected;
};