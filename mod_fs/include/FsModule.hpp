//
// FsModule.hpp for zia in /home/blazer/Epitech/Rendu/cpp_zia/include/api/HttpConf
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
#ifdef _WIN32
#ifdef ERROR
#undef ERROR
#endif
#endif
#include "api/Module/AModule.hpp"
#include "api/Logger/Logger.hh"
#include "api/HttpConf/HttpConf.hpp"
#include "MimeType.hpp"

class     FsModule : public apouche::AModule
{
public:
  FsModule(apouche::Logger * = new apouche::Logger());
  ~FsModule();

  void registerEvents(apouche::EventHandler *);

  apouche::ReturnState execute(apouche::IHttpRequest *, apouche::IHttpResponse *, apouche::IHttpConf *);

private:
  const std::string *_getDocumentRoot(const HttpConf *);
  bool _getIndexDirectory(const HttpConf *);
  void _printNotFound(apouche::IHttpResponse *);
  void _printForbidden(apouche::IHttpResponse *);
  bool _printResource(apouche::IHttpResponse *, const std::string &);
  bool _setMimeType(apouche::IHttpResponse *, const std::string &);
  void _printIndexDirectory(apouche::IHttpResponse *, const std::string &, bool, const std::string &);
  const std::string _getExtension(const std::string &);
  const std::string _paddedPath(const std::string &);
  const std::string _getUri(const std::string &endpoint);
  void _openHtml(std::string &);
  void _closeHtml(std::string &);

private:
  MimeType _mimeType;
};

extern "C" apouche::AModule *instantiate() {
  return new FsModule();
}

extern "C" apouche::AModule *instantiateWithLogger(apouche::Logger *log){
  return new FsModule(log);
}