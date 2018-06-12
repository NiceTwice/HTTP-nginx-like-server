//
// ParserUtils.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Wed Feb 01 11:25:00 2017 Adrien Vasseur
// Last update Wed Feb 01 11:25:00 2017 Adrien Vasseur
//

#pragma   once

#include  <string>
#include  <vector>

class     ParserUtils
{
public:

  static void split(std::vector<std::string> &, const std::string &, char);

  static bool parsePort(std::uint16_t &, const std::string &);

  static void parsePorts(std::vector<std::uint16_t> &, const std::vector<std::string> &);
};