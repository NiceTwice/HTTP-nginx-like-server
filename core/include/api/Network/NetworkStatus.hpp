//
// NetworkStatus.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Tue Jan 31 21:04:27 2017 Adrien Vasseur
// Last update Tue Jan 31 21:04:27 2017 Adrien Vasseur
//

#pragma   once

#include  "api/Network/INetworkStatus.hpp"

class     NetworkStatus : public apouche::INetworkStatus
{
public:
  NetworkStatus();
  ~NetworkStatus();

  std::vector<apouche::IZiaConnection *> const &getConnections(Treatment const &) const;
  void addConnection(apouche::IZiaConnection *, Treatment const &);
  void clear();

private:
  std::vector<apouche::IZiaConnection *>  _input;
  std::vector<apouche::IZiaConnection *>  _output;
};