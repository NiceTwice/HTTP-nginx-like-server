//
// Main.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Fri Jan 20 18:11:17 2017 Adrien Vasseur
// Last update Fri Jan 20 18:11:17 2017 Adrien Vasseur
//

#include  <iostream>
#include  <csignal>
#ifdef _WIN32
#include    <winsock2.h>
#include    <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#endif
#include  "Core.hpp"

static void stopSignal(int)
{
  Core::stop();
}

int     main(int, char **)
{
  Core  core;

  /*
   * Start WinsockAPI 
   */
#ifdef _WIN32
  WSADATA wsaData;
  int iResult;

  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != NO_ERROR)
  {
    std::cerr << "Error at WSAStartup()" << std::endl;
  }
#endif

  if (!core.init())
    return (1);

#ifndef _WIN32
  signal(SIGPIPE, SIG_IGN);
#endif
  signal(SIGINT, &stopSignal);

  core.run();
  return (0);
}