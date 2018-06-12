//
// SSLSocket.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Wed Feb 01 14:48:32 2017 Adrien Vasseur
// Last update Wed Feb 01 14:48:32 2017 Adrien Vasseur
//

#include <iostream>
#include    "SSLSocket.hpp"

SSLSocket::SSLSocket()
{
  this->_ssl = NULL;
  this->_sslConnected = false;
}

SSLSocket::~SSLSocket()
{
  this->close();
}

void        SSLSocket::close()
{
  if (this->_ssl)
  {
    SSL_shutdown(this->_ssl);
    SSL_free(this->_ssl);
  }
  this->_ssl = NULL;
}

apouche::ReturnState    SSLSocket::handshake(SSL_CTX *ctx, BIO *bio, apouche::IZiaConnection *connection)
{
  int                   ret;
  int                   error;

  if (this->_ssl == NULL)
  {
    if ((this->_ssl = SSL_new(ctx)) == NULL)
      return (apouche::FATAL_ERROR);
    else if (SSL_set_fd(this->_ssl, connection->getNativeHandle().getHandler()) != 1)
      return (apouche::FATAL_ERROR);
    SSL_set_accept_state(this->_ssl);
  }
  if ((ret = SSL_do_handshake(this->_ssl)) == 1)
  {
    this->_sslConnected = true;
    return (apouche::SUCCESS);
  }
  else if ((error = SSL_get_error(this->_ssl, ret)) == SSL_ERROR_WANT_WRITE)
  {
    connection->waitToWrite(true);
    return (apouche::SUCCESS);
  }
  else if (error != SSL_ERROR_WANT_READ)
  {
    ERR_print_errors(bio);
    return (apouche::FATAL_ERROR);
  }
  return (apouche::SUCCESS);
}

apouche::ReturnState  SSLSocket::readData(apouche::IZiaConnection *connection)
{
  std::uint64_t lenght = connection->getNativeHandle().bytesAvailable();
  std::int64_t  ret;
  int           err;
  Buffer::Data  *input;

  if (lenght > 0)
  {
    input = Buffer::createData(lenght);
    ret = SSL_read(this->_ssl, input->data, lenght);
    err = SSL_get_error(this->_ssl, ret);
    if (ret <= 0 && err != SSL_ERROR_WANT_READ)
      return (apouche::FATAL_ERROR);
    input->size = (std::uint64_t) ret;
    connection->getInput() << input;
    return (apouche::SUCCESS);
  }
  return (apouche::FATAL_ERROR);
}

apouche::ReturnState  SSLSocket::writeData(apouche::IZiaConnection *connection)
{
  Buffer::Data        out;
  std::int64_t        ret;
  int           err;

  out.size = 0;
  connection->getOutput() >> &out;
  if (out.size > 0)
  {
    ret = SSL_write(this->_ssl, out.data, out.size);
    err = SSL_get_error(this->_ssl, ret);
    if (ret <= 0 && err != SSL_ERROR_WANT_WRITE)
      return (apouche::FATAL_ERROR);
    else if (ret > 0)
      connection->getOutput().removeFront(ret);
    if (!connection->getOutput().hasData())
    {
      this->close();
      return (apouche::FATAL_ERROR);
    }
    return (apouche::SUCCESS);
  }
  return (apouche::FATAL_ERROR);
}

bool        SSLSocket::isSslConnected()
{
  return (this->_sslConnected);
}