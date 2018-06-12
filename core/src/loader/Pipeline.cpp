//
// Pipeline.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sat Feb 04 15:26:47 2017 Adrien Vasseur
// Last update Sat Feb 04 15:26:47 2017 Adrien Vasseur
//

#include  "loader/Pipeline.hpp"
#include  "api/HttpConf/HttpConf.hpp"

Pipeline::Pipeline(apouche::IHttpConf *conf) : _config(conf)
{
}

Pipeline::~Pipeline()
{
}

bool      Pipeline::init()
{
  const std::string *modules;

  return (!((modules = this->_config->getConf("modules")) == NULL ||
          !this->_factory.loadModules(modules)));
}

apouche::IHttpConf  *Pipeline::getConfig() const
{
  return (this->_config);
}

apouche::EventHandler   *Pipeline::getPipeline()
{
  return (this->_factory.getEventHandler());
}