//
// Parser.cpp for JSONParser in /home/lanquemar/rendu/JSONParser/src/
//
// Made by Adrien Vasseur
// Login   <vasseu_g@epitech.eu>
//
// Started on  Sun Dec 11 21:00:39 2016 Adrien Vasseur
// Last update Mon Dec 12 22:15:16 2016 Adrien Vasseur
//

#include	"json/Parser.hpp"

Parser::Parser()
{
  this->_pos = 0;
  this->_ignoreSpaces = false;
  this->_capture = false;
}

Parser::~Parser()
{

}

const std::string	&Parser::getToken() const
{
  return (this->_token);
}

void		Parser::pushState()
{
  this->_states.push(this->_pos);
}

void		Parser::popState(bool reset)
{
  std::size_t	last;

  last = 0;
  if (this->_states.size() > 0)
    {
      last = this->_states.top();
      this->_states.pop();
    }
  if (!reset)
    this->_pos = last;
}

void		Parser::feed(const std::string &data)
{
  this->_input += data;
}

void		Parser::setIgnoreSpaces(bool value)
{
  this->_ignoreSpaces = value;
}

bool		Parser::getIgnoreSpaces() const
{
  return (this->_ignoreSpaces);
}

void		Parser::setCapture(bool capture)
{
  this->_capture = capture;
  if (capture)
    this->_token.clear();
}

bool		Parser::readChar()
{
  if (getIgnoreSpaces())
    ignoreSpaces();
  if (this->_pos >= this->_input.size())
    return (false);
  if (this->_capture)
    this->_token += this->_input[this->_pos];
  this->_pos++;
  return (true);
}

bool		Parser::readChar(char c)
{
  if (getIgnoreSpaces())
    ignoreSpaces();
  if (this->_pos >= this->_input.size() ||
      this->_input[this->_pos] != c)
    return (false);
  if (this->_capture)
    this->_token += this->_input[this->_pos];
  this->_pos++;
  return (true);
}

bool		Parser::readChar(const std::string &alpha)
{
  bool		ret;

  ret = false;
  for (std::size_t ite = 0; ite < alpha.size() && !ret; ite++)
    ret = readChar(alpha[ite]);
  return (ret);
}

bool		Parser::readEOF()
{
  if (getIgnoreSpaces())
    ignoreSpaces();
  return (this->_pos >= this->_input.size());
}

bool		Parser::readString(const std::string &alpha)
{
  bool		ret = false;

  pushState();
  while (readChar(alpha))
    ret = true;
  popState(ret);
  return (ret);
}

bool		Parser::readInteger(const std::string &key, JSONObject *obj)
{
  bool		ret;

  setCapture(true);
  ret = readString("0123456789");
  setCapture(false);
  if (ret)
    obj->addInteger(key, getToken());
  return (ret);
}

bool		Parser::readDouble(const std::string &key, JSONObject *obj)
{
  bool		ret;

  pushState();
  setCapture(true);
  ret = readString("0123456789") && readChar('.') && readString("0123456789");
  setCapture(false);
  popState(ret);
  if (ret)
    obj->addDouble(key, getToken());
  return (ret);
}

bool		Parser::readUntil(char c)
{
  bool		ret = false;

  pushState();
  while (this->_pos < this->_input.size() &&
	 this->_input[this->_pos] != c)
    {
      if (this->_pos + 1 < this->_input.size() &&
	  	this->_input[this->_pos] == '\\' &&
	  	this->_input[this->_pos + 1] == c)
		this->_pos++;
      ret = true;
      if (this->_capture)
        this->_token += this->_input[this->_pos];
      this->_pos++;
    }
  popState(ret);
  return (ret);
}

void		Parser::ignoreSpaces()
{
  while (this->_pos < this->_input.size() &&
	 std::string(SPACES).find(this->_input[this->_pos]) != std::string::npos)
  this->_pos++;
}
