//
// HttpModule.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Tue Jan 31 11:17:29 2017 Adrien Vasseur
// Last update Tue Jan 31 11:17:29 2017 Adrien Vasseur
//

#pragma   once

#include  "api/Module/AModule.hpp"
#include  "api/Logger/Logger.hh"

class     HttpModule : public apouche::AModule
{
public:
  HttpModule(apouche::Logger * = new apouche::Logger());
  ~HttpModule();

  void    registerEvents(apouche::EventHandler *);

  apouche::ReturnState    checkRequest(apouche::IHttpRequest *, apouche::IHttpResponse *, apouche::IHttpConf *);
};

extern "C" apouche::AModule *instantiate() {
  return new HttpModule();
}

extern "C" apouche::AModule *instantiateWithLogger(apouche::Logger *log){
  return new HttpModule(log);
}
