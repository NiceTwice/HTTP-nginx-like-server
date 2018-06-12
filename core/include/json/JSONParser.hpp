//
// JSONParser.hpp for JSONParser in /home/lanquemar/rendu/JSONParser/include/
//
// Made by Adrien Vasseur
// Login   <vasseu_g@epitech.eu>
//
// Started on  Sun Dec 11 20:53:02 2016 Adrien Vasseur
// Last update Mon Dec 12 22:14:43 2016 Adrien Vasseur
//

#pragma		once

#include	<string>
#include	"json/Parser.hpp"
#include	"json/JSONObject.hpp"

class		JSONParser : public Parser
{
public:
  JSONParser();
  ~JSONParser();

  bool		parse();

  bool		readAll();
  bool		readAssigment(JSONObject *);
  bool		readKey(const std::string &, JSONObject *);
  bool		readObject(const std::string &, JSONObject *);
  bool		readArray(const std::string &, JSONObject *);
  void    clear();

  const JSONObject	&getRoot() const;

private:
  JSONObject	*_root;
};
