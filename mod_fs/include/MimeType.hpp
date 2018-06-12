//
// MimeType.hpp for zia in /home/blazer/Epitech/Rendu/cpp_zia/include/api/HttpConf
//
// Made by Tom CZAYKA
// Login        <tom.czayka@epitech.eu>
//
// Started on   mer. oct.       19 12:49:58 2016 tom.czayka
// Last update  mer. oct.       19 12:49:58 2016 tom.czayka
//

#pragma   once

#include <string>
#include <map>

class     MimeType
{
public:
  MimeType();
  ~MimeType();

public:
  const std::string get(const std::string &);

private:
  std::map<std::string, std::string> _types;
};