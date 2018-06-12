//
// ModuleFactory.hpp for zia in /home/blazer/Epitech/Rendu/cpp_zia/include/loader/
//
// Made by Tom CZAYKA
// Login        <tom.czayka@epitech.eu>
//
// Started on   mer. oct.       19 12:49:58 2016 tom.czayka
// Last update  mer. oct.       19 12:49:58 2016 tom.czayka
//

#pragma once

#include <list>
#include "api/EventHandler/EventHandler.hpp"
#include "api/Module/AModule.hpp"

class ModuleFactory
{
public:
	ModuleFactory();
	~ModuleFactory();

public:
	bool loadModule(const char *);
	bool loadModules(const std::string *);
	bool clear();
	apouche::EventHandler *getEventHandler();

private:
	void *_open(const char *);
	void *_sym(void *);
	bool _clearLibHandler();
	bool _clearModules();

private:
	apouche::Logger _logger;
	std::list<apouche::AModule *> _modules;
	std::list<void *> _libHandler;
	apouche::EventHandler _eventHandler;
};