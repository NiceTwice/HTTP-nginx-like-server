//
// PhpModuleException.cpp for ZIA in /home/cugede/work/Epitech/rendu/TEK3/ZIA/cpp_zia/core/src/Exception/
//
// Made by Quentin Guerin
// Login <quentin.guerin@epitech.net>
//
// Started on  Tue Jan 31 16:48:42 2017 Quentin Guerin
// Last update Sat Feb 04 15:18:12 2017 Quentin Guerin
//

#include "PhpModuleException.hh"

PhpModuleException::PhpModuleException(std::string const& what, apouche::StatusCode code): m_what(what), m_code(code) {
}

PhpModuleException::~PhpModuleException() throw() {
}

const char *PhpModuleException::what(void) const throw() {
    return m_what.c_str();
}

apouche::StatusCode const& PhpModuleException::code(void) const throw() {
    return m_code;
}
