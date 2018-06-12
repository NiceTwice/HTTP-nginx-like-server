//
// JSONException.hpp for rtype in /home/lanquemar/rendu/rtype
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Mon Dec 26 22:43:29 2016 Adrien Vasseur
// Last update Mon Dec 26 22:43:32 2016 Adrien Vasseur
//

#pragma     once

#include    <exception>
#include    <string>

class       JSONException : public std::exception
{
public:
  JSONException(const std::string &err) : _err(err) {}
  ~JSONException() throw() {}

  const char    *what() const throw() { return (this->_err.c_str()); }

private:
  std::string   _err;
};