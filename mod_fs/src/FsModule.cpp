//
// FsModule.cpp for zia in /home/blazer/Epitech/Rendu/cpp_zia/include/api/HttpConf
//
// Made by Tom CZAYKA
// Login        <tom.czayka@epitech.eu>
//
// Started on   mer. oct.       19 12:49:58 2016 tom.czayka
// Last update  mer. oct.       19 12:49:58 2016 tom.czayka
//


#include <fstream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "FsModule.hpp"

FsModule::FsModule(apouche::Logger *log) : AModule("mod_fs",
                                                   "fs module",
                                                   "1.0.0", log)
{
  this->_logger->debug("mod_fs -> Constructor!");
}

FsModule::~FsModule()
{
  this->_logger->debug("mod_fs -> Destructor!");
}

void FsModule::registerEvents(apouche::EventHandler *handler)
{
  auto callback = std::bind(&FsModule::execute, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
  apouche::Event<apouche::ReturnState (apouche::IHttpRequest *, apouche::IHttpResponse *, apouche::IHttpConf *)>  event("FileSystem resources", apouche::Weight::HIGHEST, callback);
  handler->_afterCreateResponse.addEvent(event);
}

const std::string FsModule::_getUri(const std::string &endpoint)
{
  size_t idx;

  if ((idx = endpoint.find("?")) == std::string::npos)
    return (endpoint);
  return (endpoint.substr(0, idx));
}

apouche::ReturnState FsModule::execute(apouche::IHttpRequest *request, apouche::IHttpResponse *response, apouche::IHttpConf *config)
{
  const std::string *docRoot;

  if ((docRoot = this->_getDocumentRoot((HttpConf *)config)) == NULL)
    return (apouche::ERROR);
  const std::string path = *docRoot + this->_getUri(request->getURI());
  if (!this->_printResource(response, path))
      this->_printIndexDirectory(response, this->_getUri(request->getURI()), this->_getIndexDirectory((HttpConf *)config), path);
  return (apouche::SUCCESS);
}

void FsModule::_printNotFound(apouche::IHttpResponse *response)
{
  std::string content;

  response->setStatus(apouche::StatusCode::NotFound);
  this->_openHtml(content);
  content += "<h1>404 - Not found</h1>\n";
  this->_closeHtml(content);
  response->getBody()->setBody(content);
}

void FsModule::_printForbidden(apouche::IHttpResponse *response)
{
  std::string content;

  response->setStatus(apouche::StatusCode::Forbidden);
  this->_openHtml(content);
  content += "<h1>403 - Forbidden</h1>\n<p>";
  content += "D&eacute;sol&eacute;, ici c'est le petit jardin secret d'Adrien.</p>\n";
  this->_closeHtml(content);
  response->getBody()->setBody(content);
}

const std::string FsModule::_paddedPath(const std::string &path)
{
  std::string res;

  res = path;
  if (path[path.length() - 1] != '/')
    res += "/";
  return (res);
}

void FsModule::_openHtml(std::string &html)
{
  html += "<!DOCTYPE html>\n";
  html += "<html>\n";
  html += "<head>\n";
  html += "<title>ZIA Project</title>\n";
  html += "</head>\n";
  html += "<body>\n";
}

void FsModule::_closeHtml(std::string &html)
{
  html += "</body>\n";
  html += "</html>";
}

void FsModule::_printIndexDirectory(apouche::IHttpResponse *response, const std::string &uri, bool indexDir, const std::string &path)
{
  DIR *dir;
  struct dirent *ent;
  struct stat info;
  std::string paddedPath;
  std::string content;

  if ((dir = opendir(path.c_str())) != NULL) {
    if (!indexDir)
      this->_printForbidden(response);
    else
    {
      paddedPath = this->_paddedPath(path);
      this->_openHtml(content);
      content += "<h1>Directory listing:</h1>\n";
      if (uri != "/")
        content += "<a href=\"" + this->_paddedPath(uri) + "..\">< Parent directory</a><br><br>\n";
      while ((ent = readdir(dir)) != NULL)
      {
        if (std::string(ent->d_name) != ".." && std::string(ent->d_name) != ".")
        {
          content += "<a href=\"" + this->_paddedPath(uri) + std::string(ent->d_name) + "\">" + std::string(ent->d_name);
          stat(std::string(paddedPath + std::string(ent->d_name)).c_str(), &info);
          if (info.st_mode & S_IFDIR)
            content += "/";
          content += "</a><br>\n";
        }
        this->_closeHtml(content);
      }
      response->getBody()->setBody(content);
    }
    closedir(dir);
  }
  else
    this->_printNotFound(response);
}

bool FsModule::_setMimeType(apouche::IHttpResponse *response, const std::string &extension)
{
  response->getHeaders()->setHeader("Content-Type", this->_mimeType.get(extension));
}

const std::string FsModule::_getExtension(const std::string &path)
{
  size_t idx;

  if ((idx = path.find_last_of(".")) == std::string::npos)
    return (std::string(""));
  return (path.substr(idx + 1));
}

bool FsModule::_printResource(apouche::IHttpResponse *response, const std::string &path)
{
  std::ifstream file;
  std::streampos size;
  char * memblock;
  struct stat info;

  stat(path.c_str(), &info);
  if (info.st_mode & S_IFDIR)
    return (false);
  file.open(path, std::ios::binary | std::ios::ate);
  if (file.is_open())
  {
    size = file.tellg();
    memblock = new char [size];
    file.seekg(0, std::ios::beg);
    file.read(memblock, size);
    this->_setMimeType(response, this->_getExtension(path));
    response->getBody()->setBody(std::string(memblock, size));
    file.close();
    return (true);
  }
  return (false);
}

const std::string *FsModule::_getDocumentRoot(const HttpConf *config)
{
  const std::string *path;

  if ((path = config->getConf("document_root.path")) == NULL)
    return (NULL);
  return (path);
}

bool FsModule::_getIndexDirectory(const HttpConf *config)
{
  const std::string *path;

  if ((path = config->getConf("document_root.index_directory")) == NULL)
    return (false);
  return ((*path == std::string("true")) ? true : false);
}
