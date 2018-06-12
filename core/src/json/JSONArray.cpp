//
// JSONArray.cpp for JSONParser in /home/lanquemar/rendu/JSONParser/src/
//
// Made by Adrien Vasseur
// Login   <vasseu_g@epitech.eu>
//
// Started on  Mon Dec 12 14:01:34 2016 Adrien Vasseur
// Last update Mon Dec 12 22:14:59 2016 Adrien Vasseur
//

#include	"json/JSONArray.hpp"
#include	"json/JSONObject.hpp"

JSONArray::JSONArray()
{

}

JSONArray::~JSONArray()
{
  for (std::vector<JSONObject *>::iterator ite = this->_objects.begin(); ite != this->_objects.end(); ite++)
    delete (*ite);
}

void				JSONArray::addObject(JSONObject *obj)
{
  this->_objects.push_back(obj);
}

const JSONObject	&JSONArray::getAt(std::size_t idx) const
{
  return (*this->_objects.at(idx));
}

std::size_t         JSONArray::size() const
{
  return (this->_objects.size());
}