//
// IOModule.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Tue Jan 31 22:46:53 2017 Adrien Vasseur
// Last update Tue Jan 31 22:46:53 2017 Adrien Vasseur
//

#pragma   once

#include  "api/Module/AModule.hpp"

class     IOModule : public apouche::AModule
{
public:
  IOModule(apouche::Logger * = new apouche::Logger());
  ~IOModule();

  void    registerEvents(apouche::EventHandler *);

  apouche::ReturnState  onIOEvent(apouche::IHttpConf *, apouche::INetworkStatus *);

private:
  apouche::ReturnState  onReadEvent(apouche::IHttpConf *, apouche::IZiaConnection *);
  apouche::ReturnState  onWriteEvent(apouche::IHttpConf *, apouche::IZiaConnection *);
};

extern "C" apouche::AModule *instantiate() {
  return new IOModule();
}

extern "C" apouche::AModule *instantiateWithLogger(apouche::Logger *log){
  return new IOModule(log);
}
