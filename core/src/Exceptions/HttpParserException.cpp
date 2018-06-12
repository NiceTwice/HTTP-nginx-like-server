//
// HttpParserException.cpp for ZIA in /home/cugede/work/Epitech/rendu/TEK3/ZIA/cpp_zia/core/src/Exception/
//
// Made by Quentin Guerin
// Login <quentin.guerin@epitech.net>
//
// Started on  Tue Jan 31 16:48:42 2017 Quentin Guerin
// Last update Tue Jan 31 16:55:11 2017 Quentin Guerin
//

#include "Exceptions/HttpParserException.hh"

HttpParserException::HttpParserException(std::string const& what, apouche::StatusCode code): m_what(what), m_code(code) {
}

HttpParserException::~HttpParserException() throw() {
}

const char *HttpParserException::what(void) const throw() {
    return m_what.c_str();
}

apouche::StatusCode const& HttpParserException::code(void) const throw() {
    return m_code;
}