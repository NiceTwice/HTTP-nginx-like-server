//
// Pipeline.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sat Feb 04 15:24:50 2017 Adrien Vasseur
// Last update Sat Feb 04 15:24:50 2017 Adrien Vasseur
//

#pragma   once

#include  "api/HttpConf/IHttpConf.hpp"
#include  "loader/ModuleFactory.hpp"

class     Pipeline
{
public:
  Pipeline(apouche::IHttpConf *);
  ~Pipeline();

  bool    init();

  apouche::IHttpConf  *getConfig() const;
  apouche::EventHandler *getPipeline();

private:
  apouche::IHttpConf  *_config;
  ModuleFactory       _factory;
};