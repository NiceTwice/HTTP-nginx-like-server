//
// JSONObject.cpp for JSONParser in /home/lanquemar/rendu/JSONParser/src/
//
// Made by Adrien Vasseur
// Login   <vasseu_g@epitech.eu>
//
// Started on  Mon Dec 12 14:04:49 2016 Adrien Vasseur
// Last update Mon Dec 12 22:15:06 2016 Adrien Vasseur
//

#include    <cstdlib>
#include <json/JSONException.hpp>
#include	"json/JSONObject.hpp"
#include	"json/JSONArray.hpp"

JSONObject::JSONObject()
{

}

JSONObject::~JSONObject()
{
  for (std::map<std::string, JSONObject *>::iterator ite = this->_objects.begin(); ite != this->_objects.end(); ite++)
    delete (ite->second);
  for (std::map<std::string, JSONArray *>::iterator ite = this->_arrays.begin(); ite != this->_arrays.end(); ite++)
    delete (ite->second);
}

void				JSONObject::addObject(const std::string &key, JSONObject *obj)
{
  this->_objects[key] = obj;
}

void				JSONObject::addArray(const std::string &key, JSONArray *obj)
{
  this->_arrays[key] = obj;
}

void				JSONObject::addString(const std::string &key, const std::string &obj)
{
  this->_strings[key] = obj;
}

void				JSONObject::addDouble(const std::string &key, const std::string &obj)
{
  this->_doubles[key] = obj;
}

void				JSONObject::addInteger(const std::string &key, const std::string &obj)
{
  this->_integers[key] = obj;
}

const JSONObject	&JSONObject::getObject(const std::string &key) const
{
  try {
    return (*this->_objects.at(key));
  } catch (std::exception &) {
    throw JSONException("Unable to find object with key: " + key);
  }
}

const JSONArray		&JSONObject::getArray(const std::string &key) const
{
  try {
    return (*this->_arrays.at(key));
  } catch (std::exception &) {
    throw JSONException("Unable to find array with key: " + key);
  }
}

const std::string	&JSONObject::getString(const std::string &key) const
{
  try {
    return (this->_strings.at(key));
  } catch (std::exception &) {
    throw JSONException("Unable to find string with key: " + key);
  }
}

const std::string	&JSONObject::getDouble(const std::string &key) const
{
  try {
    return (this->_doubles.at(key));
  } catch (std::exception &) {
    throw JSONException("Unable to find double with key: " + key);
  }
}

int                 JSONObject::getInteger(const std::string &key) const
{
  char              *out;
  long int          ret;

  try {
    ret = std::strtol(this->_integers.at(key).c_str(), &out, 10);
  } catch (std::exception &) {
    throw JSONException("Unable to find integer with key: " + key);
  }
  if (*out != '\0')
    throw JSONException("Cannot parse integer: " + this->_integers.at(key));
  else if (ret >= INT32_MAX || ret < INT32_MIN)
    throw JSONException("Not a integer: " + this->_integers.at(key));
  return ((int) ret);
}

bool                JSONObject::hasObject(const std::string &key) const
{
  return (this->_objects.find(key) != this->_objects.end());
}

bool                JSONObject::hasArray(const std::string &key) const
{
  return (this->_arrays.find(key) != this->_arrays.end());
}

bool                JSONObject::hasString(const std::string &key) const
{
  return (this->_strings.find(key) != this->_strings.end());
}

bool                JSONObject::hasDouble(const std::string &key) const
{
  return (this->_doubles.find(key) != this->_doubles.end());
}

bool                JSONObject::hasInteger(const std::string &key) const
{
  return (this->_integers.find(key) != this->_integers.end());
}