//
// Parser.hpp for JSONParser in /home/lanquemar/rendu/JSONParser/include/
//
// Made by Adrien Vasseur
// Login   <vasseu_g@epitech.eu>
//
// Started on  Sun Dec 11 20:59:04 2016 Adrien Vasseur
// Last update Mon Dec 12 22:14:47 2016 Adrien Vasseur
//

#pragma		once

#include	<string>
#include	<stack>
#include	"json/JSONObject.hpp"

#define		SPACES		(" \n\t")

class		Parser
{
public:
  Parser();
  ~Parser();

  void		feed(const std::string &);

  void		setIgnoreSpaces(bool);
  bool		getIgnoreSpaces() const;
  void		setCapture(bool);

  const std::string	&getToken() const;

  void		pushState();
  void		popState(bool);

  bool		readChar();
  bool		readChar(char);
  bool		readChar(const std::string &);
  bool		readEOF();
  bool		readString(const std::string &);
  bool		readInteger(const std::string &, JSONObject *);
  bool		readDouble(const std::string &, JSONObject *);
  bool		readUntil(char);

  void		ignoreSpaces();

protected:
  std::string	_input;
  std::string	_token;
  std::stack<std::size_t>	_states;
  std::size_t	_pos;

  bool			_ignoreSpaces;
  bool			_capture;
};
