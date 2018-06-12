//
// JSONObject.hpp for JSONParser in /home/lanquemar/rendu/JSONParser/include/
//
// Made by Adrien Vasseur
// Login   <vasseu_g@epitech.eu>
//
// Started on  Mon Dec 12 13:44:42 2016 Adrien Vasseur
// Last update Mon Dec 12 14:13:31 2016 Adrien Vasseur
//

#pragma		once

#include	<map>
#include	<string>

class		JSONArray;

class		JSONObject
{
public:
  JSONObject();
  ~JSONObject();

  void				addObject(const std::string &, JSONObject *);
  void				addArray(const std::string &, JSONArray *);
  void				addString(const std::string &, const std::string &);
  void				addDouble(const std::string &, const std::string &);
  void				addInteger(const std::string &, const std::string &);

  const JSONObject	&getObject(const std::string &) const;
  const JSONArray	&getArray(const std::string &) const;
  const std::string	&getString(const std::string &) const;
  const std::string	&getDouble(const std::string &) const;
  int               getInteger(const std::string &) const;

  bool              hasObject(const std::string &) const;
  bool              hasArray(const std::string &) const;
  bool              hasString(const std::string &) const;
  bool              hasDouble(const std::string &) const;
  bool              hasInteger(const std::string &) const;

private:
  std::map<std::string, JSONObject *>	_objects;
  std::map<std::string, JSONArray *>	_arrays;
  std::map<std::string, std::string>	_strings;
  std::map<std::string, std::string>	_doubles;
  std::map<std::string, std::string>	_integers;
};
