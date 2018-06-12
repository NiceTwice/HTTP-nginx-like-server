//
// JSONParser.cpp for JSONParser in /home/lanquemar/rendu/JSONParser/src/
//
// Made by Adrien Vasseur
// Login   <vasseu_g@epitech.eu>
//
// Started on  Sun Dec 11 20:53:49 2016 Adrien Vasseur
// Last update Mon Dec 12 22:15:12 2016 Adrien Vasseur
//

#include	<cstdlib>
#include	"json/JSONParser.hpp"
#include	"json/JSONArray.hpp"

JSONParser::JSONParser()
{
  this->_root = NULL;
}

JSONParser::~JSONParser()
{
  this->clear();
}

void    JSONParser::clear()
{
  this->_input.clear();
  this->_token.clear();
  while (!this->_states.empty())
    this->_states.pop();
  this->_pos = 0;
  if (this->_root)
    delete this->_root;
  this->_root = NULL;
}

bool		JSONParser::parse()
{
  setIgnoreSpaces(true);
  return (readAll());
}

bool		JSONParser::readAll()
{
  bool		ret;

  this->_root = new JSONObject();
  pushState();
  if ((ret = readChar('{')))
	{
	  ret = readAssigment(this->_root);
	  while (readChar(','))
	    ret = readAssigment(this->_root);
	  if (ret)
      	ret = readChar('}') && readEOF();
	}
  popState(ret);
  return (ret);
}

bool		JSONParser::readAssigment(JSONObject *parent)
{
  std::string	key;
  bool			ret;

  pushState();
  if ((ret = readKey("", NULL)))
    key = getToken();
  ret = ret && readChar(':') && (readArray(key, parent) || readObject(key, parent) || readDouble(key, parent) || readKey(key, parent) || readInteger(key, parent));
  popState(ret);
  return (ret);
}

bool		JSONParser::readKey(const std::string &key, JSONObject *obj)
{
  bool		ret;

  pushState();
  ret = readChar('"');
  setCapture(true);
  ret = ret && readUntil('"');
  setCapture(false);
  ret = ret && readChar('"');
  popState(ret);
  if (ret && obj != NULL)
    obj->addString(key, getToken());
  return (ret);
}

bool		JSONParser::readObject(const std::string &key, JSONObject *parent)
{
  JSONObject	*obj = new JSONObject();
  bool			ret;

  pushState();
  if ((ret = readChar('{')))
	{
	  ret = readAssigment(obj);
	  while (readChar(','))
	    ret = readAssigment(obj);
	  if (ret)
      	ret = readChar('}');
	}
  popState(ret);
  if (ret)
    parent->addObject(key, obj);
  else
    delete obj;
  return (ret);
}

bool		JSONParser::readArray(const std::string &key, JSONObject *parent)
{
  JSONArray		*array = new JSONArray();
  JSONObject	*obj;
  bool			ret;

  pushState();
  if ((ret = readChar('[')))
    {
      obj = new JSONObject();
      if ((ret = (readObject("value", obj) || readDouble("value", obj) || readInteger("value", obj) || readKey("value", obj))))
		array->addObject(obj);
      else
		delete obj;
      while (readChar(','))
		{
	  	  obj = new JSONObject();
		  if ((ret = (readObject("value", obj) || readDouble("value", obj) || readInteger("value", obj) || readKey("value", obj))))
	    	array->addObject(obj);
		  else
	    	delete obj;
		}
      if (ret)
		ret = readChar(']');
    }
  popState(ret);
  if (ret)
    parent->addArray(key, array);
  else
    delete array;
  return (ret);
}

const JSONObject	&JSONParser::getRoot() const
{
  return (*this->_root);
}
