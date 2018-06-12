//
// Clock.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sat Feb 04 16:41:05 2017 Adrien Vasseur
// Last update Sat Feb 04 16:41:05 2017 Adrien Vasseur
//

#pragma   once

#include  <cstdint>

class     Clock
{
public:
  static std::uint64_t  getCurrentTime();
  static std::uint64_t  getDiffTime(std::uint64_t);
};