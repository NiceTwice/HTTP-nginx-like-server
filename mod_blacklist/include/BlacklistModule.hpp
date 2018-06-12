//
// BlacklistModule.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sun Feb 05 21:46:32 2017 Adrien Vasseur
// Last update Sun Feb 05 21:46:32 2017 Adrien Vasseur
//

#pragma   once

#include  "api/Module/AModule.hpp"
#include  "api/Logger/Logger.hh"

class     BlacklistModule : public apouche::AModule
{
public:
  BlacklistModule(apouche::Logger * = new apouche::Logger());
  ~BlacklistModule();

  void    registerEvents(apouche::EventHandler *);

  bool    loadFile(const std::string &);
  bool    isBlacklisted(const std::string &);

  apouche::ReturnState  onConnect(apouche::IZiaConnection *, apouche::IHttpConf *);

private:
  bool                        _loaded;
  std::map<std::string, int>  _blacklisted;
};

extern "C" apouche::AModule *instantiate() {
  return new BlacklistModule();
}

extern "C" apouche::AModule *instantiateWithLogger(apouche::Logger *log){
  return new BlacklistModule(log);
}