//
// PhpModule.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Tue Jan 31 11:17:29 2017 Adrien Vasseur
// Last update Fri Feb 03 16:17:10 2017 Quentin Guerin
//

#pragma once

#include <map>
#include <string>
#include "api/Module/AModule.hpp"
#include "api/Logger/Logger.hh"
#include "PhpModuleException.hh"

class PhpModule : public apouche::AModule
{
public:
    PhpModule(apouche::Logger * = new apouche::Logger());
    ~PhpModule();

    void registerEvents(apouche::EventHandler *);

    apouche::ReturnState execute(apouche::IHttpRequest *, apouche::IHttpResponse *, apouche::IHttpConf *);

private:
    std::string _prepareCommand(apouche::IHttpRequest *, apouche::IHttpConf *, char ***) const throw(PhpModuleException);

private:
    static const std::map<apouche::Method, std::string> _methodAsString;
};

extern "C" apouche::AModule *instantiate() {
    return new PhpModule();
}

extern "C" apouche::AModule *instantiateWithLogger(apouche::Logger *log) {
    return new PhpModule(log);
}