//
// JSONArray.hpp for JSONParser in /home/lanquemar/rendu/JSONParser/include/
//
// Made by Adrien Vasseur
// Login   <vasseu_g@epitech.eu>
//
// Started on  Mon Dec 12 13:49:45 2016 Adrien Vasseur
// Last update Mon Dec 12 14:11:19 2016 Adrien Vasseur
//

#pragma		once

#include	<vector>

class		JSONObject;

class		JSONArray
{
public:
  JSONArray();
  ~JSONArray();

  void				addObject(JSONObject *);
  const JSONObject	&getAt(std::size_t) const;
  std::size_t       size() const;

private:
  std::vector<JSONObject *>	_objects;
};
