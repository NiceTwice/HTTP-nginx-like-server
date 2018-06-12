//
// IOModule.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Tue Jan 31 22:47:40 2017 Adrien Vasseur
// Last update Tue Jan 31 22:47:40 2017 Adrien Vasseur
//

#include    "IOModule.hpp"

IOModule::IOModule(apouche::Logger *log) : AModule("mod_io",
                               "Network input/output module manager",
                               "1.0.0", log)
{
  this->_logger->debug("mod_io -> Constructor!");
}

IOModule::~IOModule()
{
  this->_logger->debug("mod_io -> Destructor!");
}

void        IOModule::registerEvents(apouche::EventHandler *handler)
{
  auto callback = std::bind(&IOModule::onIOEvent, this, std::placeholders::_1, std::placeholders::_2);
  apouche::Event<apouche::ReturnState (apouche::IHttpConf *, apouche::INetworkStatus *)> event("New input/output event", apouche::Weight::HIGH, callback);
  handler->_onNetworkIO.addEvent(event);
}

apouche::ReturnState  IOModule::onReadEvent(apouche::IHttpConf *conf, apouche::IZiaConnection *connection)
{
  const std::string   *ssl;
  bool                enabled = true;

  if (conf != NULL)
  {
    ssl = conf->getConf("server.port." + std::to_string(connection->getNativeHandle().getPort()) + ".ssl");
    if (ssl != NULL && (*ssl).compare("true") == 0)
      enabled = false;
  }
  if (enabled)
    return ((apouche::ReturnState) connection->ReceiveRequest());
  return (apouche::SUCCESS);
}

apouche::ReturnState  IOModule::onWriteEvent(apouche::IHttpConf *conf, apouche::IZiaConnection *connection)
{
  const std::string   *ssl;
  bool                enabled = true;

  if (conf != NULL)
  {
    ssl = conf->getConf("server.port." + std::to_string(connection->getNativeHandle().getPort()) + ".ssl");
    if (ssl != NULL && (*ssl).compare("true") == 0)
      enabled = false;
  }
  if (enabled)
    return ((apouche::ReturnState) connection->SendResponse(NULL));
  return (apouche::SUCCESS);
}

apouche::ReturnState  IOModule::onIOEvent(apouche::IHttpConf *conf, apouche::INetworkStatus *network)
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