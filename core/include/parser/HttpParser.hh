//
// HttpParser.hh for ZIA in /home/cugede/work/Epitech/rendu/TEK3/ZIA/cpp_zia/core/include/parser
//
// Made by Quentin Guerin
// Login <quentin.guerin@epitech.net>
//
// Started on  Tue Jan 31 16:48:42 2017 Quentin Guerin
// Last update Tue Jan 31 16:55:11 2017 Quentin Guerin
//

#pragma once

# include <string>
# include <sstream>
# include "api/HttpResponse/IHttpResponse.hh"
# include "net/Buffer.hpp"
# include "api/Enum.hh"
# include "api/HttpRequest/IHttpRequest.hh"
# include "api/Logger/FileLogger.hh"
# include "Exceptions/HttpParserException.hh"

class HttpParser
{
public:
    HttpParser(void);
    ~HttpParser(void);

public:
    bool parseRequest(apouche::IHttpRequest *, std::string const&) throw(HttpParserException);

public:
    static std::stringstream &getline(std::stringstream&, std::string&, std::string const&, bool&);
    static bool checkUri(std::string const&);
    static bool checkHttpVersion(std::string &);
    static bool parseHeaderLine(std::string const&, std::string&, std::string&);
    static bool parseStartLine(apouche::IHttpRequest*, std::string &) throw(HttpParserException);
    static Buffer::Data *toData(apouche::IHttpResponse const *);

protected:
    static apouche::Method _getMethodFromName(std::string const&);

protected:
    static const std::map<std::string, apouche::Method> m_methodsFromName;

protected:
    apouche::Logger m_logger;
};
