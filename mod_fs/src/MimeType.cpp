//
// MimeType.cpp for zia in /home/blazer/Epitech/Rendu/cpp_zia/include/api/HttpConf
//
// Made by Tom CZAYKA
// Login        <tom.czayka@epitech.eu>
//
// Started on   mer. oct.       19 12:49:58 2016 tom.czayka
// Last update  mer. oct.       19 12:49:58 2016 tom.czayka
//

#include "MimeType.hpp"

MimeType::MimeType()
{
  this->_types["html"] = "text/html";
  this->_types["css"] = "text/css";
  this->_types["js"] = "application/javascript";
  this->_types["gif"] = "image/gif";
  this->_types["png"] = "image/png";
  this->_types["jpg"] = "image/jpeg";
  this->_types["jpeg"] = "image/jpeg";
  this->_types["pdf"] = "application/pdf";
  this->_types["xml"] = "text/xml";
  this->_types["csv"] = "text/csv";
  this->_types["mp4"] = "video/mp4";
  this->_types["mp3"] = "audio/mpeg";
  this->_types["mpeg"] = "audio/mpeg";
  this->_types["wav"] = "audio/x-wav";
  this->_types["zip"] = "application/zip";
  this->_types["json"] = "application/json";
}

MimeType::~MimeType()
{

}

const std::string MimeType::get(const std::string &extension)
{
	if (this->_types.find(extension) == this->_types.end())
		return (std::string("text/plain"));
	return (this->_types[extension]);
}