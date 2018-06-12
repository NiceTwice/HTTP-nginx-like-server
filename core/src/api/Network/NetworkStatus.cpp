//
// NetworkStatus.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Tue Jan 31 21:06:30 2017 Adrien Vasseur
// Last update Tue Jan 31 21:06:30 2017 Adrien Vasseur
//

#include    "api/Network/NetworkStatus.hpp"

NetworkStatus::NetworkStatus()
{
}

NetworkStatus::~NetworkStatus()
{
}

std::vector<apouche::IZiaConnection *> const &NetworkStatus::getConnections(Treatment const &mode) const
{
  if (mode == apouche::INetworkStatus::WRITE)
    return (this->_output);
  return (this->_input);
}

void        NetworkStatus::addConnection(apouche::IZiaConnection *connection, Treatment const &mode)
{
  if (mode == apouche::INetworkStatus::WRITE)
    this->_output.push_back(connection);
  else
    this->_input.push_back(connection);
}

void        NetworkStatus::clear()
{
  this->_input.clear();
  this->_output.clear();
}