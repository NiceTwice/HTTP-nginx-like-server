//
// ZiaMD5.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sat Feb 04 15:44:55 2017 Adrien Vasseur
// Last update Sat Feb 04 15:44:55 2017 Adrien Vasseur
//

#pragma   once

#include  <string>
#include  <openssl/md5.h>

class     ZiaMD5
{
public:
  ZiaMD5();
  ZiaMD5(const ZiaMD5 &);
  ~ZiaMD5();

  const ZiaMD5  &operator=(const ZiaMD5 &);

  bool    operator==(const ZiaMD5 &) const;
  bool    operator!=(const ZiaMD5 &) const;

  void    feed(const std::string &);
  void    calc();

  const unsigned char *getResult() const;
  const std::string   &getBuffer() const;
  bool    isEmpty() const;

private:
  std::string   _buff;
  unsigned char _result[MD5_DIGEST_LENGTH];
};