//
// Clock.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sat Feb 04 16:43:15 2017 Adrien Vasseur
// Last update Sat Feb 04 16:43:15 2017 Adrien Vasseur
//

#include        <chrono>
#include        "thread/Clock.hpp"

std::uint64_t   Clock::getCurrentTime()
{
  std::chrono::steady_clock::time_point	now = std::chrono::steady_clock::now();
  return ((std::uint64_t) std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
}

std::uint64_t   Clock::getDiffTime(std::uint64_t last)
{
  return (Clock::getCurrentTime() - last);
}