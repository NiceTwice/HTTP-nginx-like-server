//
// ModuleFactory.cpp for zia in /home/blazer/Epitech/Rendu/cpp_zia/src/loader/
//
// Made by Tom CZAYKA
// Login        <tom.czayka@epitech.eu>
//
// Started on   mer. oct.       19 12:49:58 2016 tom.czayka
// Last update  mer. oct.       19 12:49:58 2016 tom.czayka
//

#include <cstdlib>
#include <sstream>
#ifdef _WIN32
# include <windows.h>
# elif __unix__
# include <dlfcn.h>
#endif
#include "loader/ModuleFactory.hpp"

ModuleFactory::ModuleFactory()
{

}

ModuleFactory::~ModuleFactory()
{
	this->clear();
}

void *ModuleFactory::_open(const char *path)
{
#ifndef _WIN32
  return (dlopen(path, RTLD_NOW));
#else
  return (LoadLibrary(path));
#endif
}

void *ModuleFactory::_sym(void *handler)
{
#ifndef _WIN32
  return (dlsym(handler, "instantiateWithLogger"));
#else
  return ((void *)GetProcAddress((HINSTANCE) handler, "instantiateWithLogger"));
#endif
}

bool ModuleFactory::loadModule(const char *path)
{
    void *handler;
    void *symbol;
    apouche::AModule *module;

    if ((handler = this->_open(path)) == NULL)
        return (false);
    this->_libHandler.push_back(handler);
    if ((symbol = this->_sym(handler)) == NULL)
        return (false);
    module = (*(apouche::AModule *(*)(apouche::Logger *))symbol)(&(this->_logger));
    this->_modules.push_back(module);
    module->registerEvents(&(this->_eventHandler));
    return (true);
}

bool ModuleFactory::loadModules(const std::string *modules)
{
	std::istringstream issModules(*modules);

	for (std::string module; std::getline(issModules, module, ',');)
	{
		if (this->loadModule(module.c_str()))
			std::cout << "[LOADER] OK: " << module << std::endl;
		else
		{
			std::cout << "[LOADER] KO: " << module << std::endl;
			std::cerr << "[ERROR] Unable to load modules" << std::endl;
			return (false);
		}
	}
	return (true);
}

bool ModuleFactory::_clearLibHandler()
{
  std::list<void *>::iterator ite;

	for (ite = this->_libHandler.begin(); ite != this->_libHandler.end(); ++ite)
	{
#ifndef _WIN32
	  dlclose(*ite);
#else
          FreeLibrary((HINSTANCE)*ite);
#endif
	}
	this->_libHandler.clear();
	return (true);
}

bool ModuleFactory::_clearModules()
{
	std::list<apouche::AModule *>::iterator ite;

	for (ite = this->_modules.begin(); ite != this->_modules.end(); ++ite)
	{
		delete *ite;
	}
	this->_modules.clear();
	return (true);
}

bool ModuleFactory::clear()
{
	std::list<void *>::iterator ite;

	this->_eventHandler.clear();
	this->_clearModules();
	this->_clearLibHandler();
	return (true);
}

apouche::EventHandler *ModuleFactory::getEventHandler()
{
	return (&(this->_eventHandler));
}