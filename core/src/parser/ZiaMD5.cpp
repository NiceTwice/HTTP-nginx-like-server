//
// ZiaMD5.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sat Feb 04 15:48:12 2017 Adrien Vasseur
// Last update Sat Feb 04 15:48:12 2017 Adrien Vasseur
//

#include  "parser/ZiaMD5.hpp"

ZiaMD5::ZiaMD5()
{
}

ZiaMD5::ZiaMD5(const ZiaMD5 &cpy)
{
  this->_buff = cpy.getBuffer();
  for (size_t i = 0; i < MD5_DIGEST_LENGTH; i++)
    this->_result[i] = cpy.getResult()[i];
}

ZiaMD5::~ZiaMD5()
{
}

const ZiaMD5  &ZiaMD5::operator=(const ZiaMD5 &cpy)
{
  this->_buff = cpy.getBuffer();
  for (size_t i = 0; i < MD5_DIGEST_LENGTH; i++)
    this->_result[i] = cpy.getResult()[i];
  return (*this);
}

bool      ZiaMD5::operator==(const ZiaMD5 &ope) const
{
  const unsigned char *other = ope.getResult();

  for (std::size_t i = 0; i < MD5_DIGEST_LENGTH; i++)
    if (this->_result[i] != other[i])
      return (false);
  return (true);
}

bool      ZiaMD5::operator!=(const ZiaMD5 &ope) const
{
  return (!((*this) == ope));
}

void      ZiaMD5::feed(const std::string &buff)
{
  this->_buff += buff;
}

void      ZiaMD5::calc()
{
  MD5((unsigned char*) this->_buff.c_str(), this->_buff.size(), this->_result);
}

const unsigned char *ZiaMD5::getResult() const
{
  return (this->_result);
}

const std::string   &ZiaMD5::getBuffer() const
{
  return (this->_buff);
}

bool      ZiaMD5::isEmpty() const
{
  return (this->_buff.size() == 0);
}