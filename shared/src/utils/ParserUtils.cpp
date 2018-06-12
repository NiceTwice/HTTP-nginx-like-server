//
// ParserUtils.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Wed Feb 01 11:26:10 2017 Adrien Vasseur
// Last update Wed Feb 01 11:26:10 2017 Adrien Vasseur
//

#include    <iostream>
#include    <sstream>
#include    <stdlib.h>
#include    "utils/ParserUtils.hpp"

void        ParserUtils::split(std::vector<std::string> &out, const std::string &in, char delim)
{
  std::istringstream iss(in);

  out.clear();
  for (std::string token; std::getline(iss, token, delim);)
    out.push_back(token);
}

bool        ParserUtils::parsePort(std::uint16_t &out, const std::string &in)
{
  long int  ret;
  char      *endptr;

  ret = std::strtol(in.c_str(), &endptr, 10);
  if (endptr[0] != '\0' || ret < 0 || ret >= 65535)
    return (false);
  out = (std::uint16_t) ret;
  return (true);
}

void        ParserUtils::parsePorts(std::vector<std::uint16_t> &out, const std::vector<std::string> &in)
{
  std::uint16_t port;

  out.clear();
  for (auto ite = in.begin(); ite != in.end(); ite++)
  {
    if (!ParserUtils::parsePort(port, (*ite)))
      std::cerr << "[WARN] Unable to parse port: " << (*ite) << std::endl;
    else
      out.push_back(port);
  }
}