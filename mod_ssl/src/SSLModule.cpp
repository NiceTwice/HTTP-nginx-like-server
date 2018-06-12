//
// SSLModule.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Wed Feb 01 13:10:35 2017 Adrien Vasseur
// Last update Wed Feb 01 13:10:35 2017 Adrien Vasseur
//

#include  <atomic>
#include  "utils/ParserUtils.hpp"
#include  "SSLModule.hpp"

std::atomic<int>  _loaded;

SSLModule::SSLModule(apouche::Logger *log) : AModule("mod_ssl",
                                                     "Network input/output ssl module manager",
                                                     "1.0.0", log)
{
  this->_initialized = false;
  this->initSSL();
}

SSLModule::~SSLModule()
{
  for (auto ite = this->_contexts.begin(); ite != this->_contexts.end(); ite++)
  {
    this->freeCTX(ite->second->_ssl, ite->second->_bio);
    delete ite->second;
  }
  this->freeSSL();
}

void      SSLModule::initSSL()
{
  int     first = (_loaded++);

  if (first == 0)
  {
    SSL_load_error_strings();
    SSL_library_init();
  }
}

void      SSLModule::freeSSL()
{
  int     last = (--_loaded);

  if (last == 0)
  {
    SSL_COMP_free_compression_methods();
    ERR_remove_state(0);
    ERR_free_strings();
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
  }
}

void      SSLModule::createCTX(const std::string &key, const std::string &cert, std::uint16_t port)
{
  Context *ctx = new Context;

  this->_logger->info("Creating SSL context on port " + std::to_string(port) + "...");
  if ((ctx->_ssl = SSL_CTX_new(SSLv23_method())) == NULL)
    this->_logger->error("SSL_CTX_new failed");
  else if ((ctx->_bio = BIO_new_fd(2, BIO_NOCLOSE)) == NULL)
    this->_logger->error("BIO_new_fd failed");
  else if (SSL_CTX_use_certificate_file(ctx->_ssl, cert.c_str(), SSL_FILETYPE_PEM) != 1)
    this->_logger->error("SSL_CTX_use_certificate_file failed");
  else if (SSL_CTX_use_PrivateKey_file(ctx->_ssl, key.c_str(), SSL_FILETYPE_PEM) != 1)
    this->_logger->error("SSL_CTX_use_PrivateKey_file failed");
  else if (SSL_CTX_check_private_key(ctx->_ssl) != 1)
    this->_logger->error("SSL_CTX_check_private_key failed");
  else
  {
    this->_contexts[port] = ctx;
    this->_logger->info("SSL Context created!");
    return;
  }
  this->freeCTX(ctx->_ssl, ctx->_bio);
  delete ctx;
}

void      SSLModule::freeCTX(SSL_CTX *ctx, BIO *bio)
{
  if (bio)
    BIO_free(bio);
  if (ctx)
    SSL_CTX_free(ctx);
}

SSLModule::Context   *SSLModule::getContext(std::uint16_t port)
{
  std::map<std::uint16_t, Context *>::iterator ite = this->_contexts.find(port);

  if (ite != this->_contexts.end())
    return (ite->second);
  return (NULL);
}

void      SSLModule::createContextOnPort(apouche::IHttpConf *conf, std::uint16_t port)
{
  const std::string   *ssl = conf->getConf("server.port." + std::to_string(port) + ".ssl");

  if (ssl != NULL)
  {
    if ((*ssl).compare("true") == 0)
    {
      const std::string   *key = (conf->getConf("server.port." + std::to_string(port) + ".parameters.server_key"));
      const std::string   *cert = (conf->getConf("server.port." + std::to_string(port) + ".parameters.server_cert"));

      std::string         keyFilename = "key.pem";
      std::string         certFilename = "cert.pem";

      if (key != NULL)
        keyFilename = *key;
      if (cert != NULL)
        certFilename = *cert;
      this->createCTX(keyFilename, certFilename, port);
    }
  }
}

void      SSLModule::createContexts(apouche::IHttpConf *config)
{
  const std::string *conf = config->getConf("server.port");
  std::vector<std::string>  ports;
  std::vector<std::uint16_t>  parsedPorts;

  this->_initialized = true;
  if (conf == NULL)
    return ;
  ParserUtils::split(ports, *conf, ',');
  if (ports.size() <= 0)
    return ;
  ParserUtils::parsePorts(parsedPorts, ports);
  if (parsedPorts.size() <= 0)
    return ;
  for (auto ite = parsedPorts.begin(); ite != parsedPorts.end(); ite++)
    this->createContextOnPort(config, (*ite));
}

void      SSLModule::registerEvents(apouche::EventHandler *handler)
{
  auto callback = std::bind(&SSLModule::onIOEvent, this, std::placeholders::_1, std::placeholders::_2);
  apouche::Event<apouche::ReturnState (apouche::IHttpConf *, apouche::INetworkStatus *)> event("New input/output event", apouche::Weight::HIGH, callback);
  handler->_onNetworkIO.addEvent(event);
}

apouche::ReturnState  SSLModule::onReadEvent(apouche::IHttpConf *conf, apouche::IZiaConnection *connection)
{
  const std::string   *ssl;
  bool                enabled = false;
  std::uint16_t       port = connection->getNativeHandle().getPort();

  if (conf != NULL)
  {
    if (!this->_initialized)
      this->createContexts(conf);
    ssl = conf->getConf("server.port." + std::to_string(port) + ".ssl");
    if (ssl != NULL && (*ssl).compare("true") == 0)
      enabled = true;
  }
  if (enabled)
    return (this->handleRead(connection, port));
  return (apouche::SUCCESS);
}

apouche::ReturnState  SSLModule::onWriteEvent(apouche::IHttpConf *conf, apouche::IZiaConnection *connection)
{
  const std::string   *ssl;
  bool                enabled = false;
  std::uint16_t       port = connection->getNativeHandle().getPort();

  if (conf != NULL)
  {
    if (!this->_initialized)
      this->createContexts(conf);
    ssl = conf->getConf("server.port." + std::to_string(port) + ".ssl");
    if (ssl != NULL && (*ssl).compare("true") == 0)
      enabled = true;
  }
  if (enabled)
    return (this->handleWrite(connection, port));
  return (apouche::SUCCESS);
}

apouche::ReturnState    SSLModule::onIOEvent(apouche::IHttpConf *conf, apouche::INetworkStatus *network)
{
  apouche::ReturnState ret = apouche::FATAL_ERROR;
  std::vector<apouche::IZiaConnection *>  connections;

  connections = network->getConnections(apouche::INetworkStatus::READ);
  for (auto ite = connections.begin(); ite != connections.end(); ite++)
    ret = this->onReadEvent(conf, *ite);
  connections = network->getConnections(apouche::INetworkStatus::WRITE);
  for (auto ite = connections.begin(); ite != connections.end(); ite++)
    ret = this->onWriteEvent(conf, *ite);
  return (ret);
}

apouche::ReturnState  SSLModule::handleRead(apouche::IZiaConnection *connection, std::uint16_t port)
{
  apouche::ReturnState  ret;
  SSLSocket           *ssl;
  Context             *ctx;

  ssl = (SSLSocket *) connection->getData();
  if (ssl == NULL)
  {
    connection->setData((void *) new SSLSocket);
    return (apouche::SUCCESS);
  }
  else if (!ssl->isSslConnected())
  {
    if ((ctx = this->getContext(port)) == NULL)
      return (apouche::FATAL_ERROR);
    return (ssl->handshake(ctx->_ssl, ctx->_bio, connection));
  }
  if ((ret = ssl->readData(connection)) == apouche::FATAL_ERROR)
  {
    ssl->close();
    connection->setData(NULL);
  }
  return (ret);
}

apouche::ReturnState  SSLModule::handleWrite(apouche::IZiaConnection *connection, std::uint16_t port)
{
  apouche::ReturnState  ret;
  SSLSocket           *ssl;
  Context             *ctx;

  ssl = (SSLSocket *) connection->getData();
  connection->waitToWrite(false);
  if (ssl == NULL)
  {
    connection->setData((void *) new SSLSocket);
    return (apouche::SUCCESS);
  }
  else if (!ssl->isSslConnected())
  {
    if ((ctx = this->getContext(port)) == NULL)
      return (apouche::FATAL_ERROR);
    return (ssl->handshake(ctx->_ssl, ctx->_bio, connection));
  }
  if ((ret = ssl->writeData(connection)) == apouche::FATAL_ERROR)
  {
    ssl->close();
    connection->setData(NULL);
  }
  return (ret);
}