//
// HttpParser.cpp for ZIA in /home/cugede/work/Epitech/rendu/TEK3/ZIA/cpp_zia/core/src/parser
//
// Made by Quentin Guerin
// Login <quentin.guerin@epitech.net>
//
// Started on  Tue Jan 31 16:48:02 2017 Quentin Guerin
// Last update Wed Feb 01 09:48:03 2017 Quentin Guerin
//

#include <locale>
#include <istream>
#include <iostream>
#include <regex>
#include "parser/HttpParser.hh"

HttpParser::HttpParser(void)
{
}

HttpParser::~HttpParser(void)
{
}

bool HttpParser::parseHeaderLine(std::string const& header,
                                 std::string &key, std::string &value)
{
    size_t pos;
    std::string tmpKey;
    std::string tmpValue;

    if ((pos = header.find(": ")) == std::string::npos)
        return (false);
    tmpKey = header.substr(0, pos);
    tmpValue = header.substr(pos + 2, std::string::npos);
    if (tmpKey.size() == 0 || tmpValue.size() == 0)
        return (false);
    for (auto const& c: tmpKey) {
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == '-' || c == ':'))
            return (false);
    }
    for (auto const& c: tmpValue) {
        if ((c >= 0x00 && c <= 0x0f) || c == 0x7f)
            return (false);
    }
    key = tmpKey;
    value = tmpValue;
    return (true);
}

bool HttpParser::checkUri(std::string const& uri)
{
    std::smatch uri_match_result;
    static const std::regex uri_regex(R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)",
                                      std::regex::extended);

    if (std::regex_match(uri, uri_match_result, uri_regex))
        return (true);
    else
        return (false);
}

bool HttpParser::checkHttpVersion(std::string &version)
{
    std::string substr;

    if (version.size() > std::string("HTTP/1.").size() &&
        version.substr(0, std::string("HTTP/1.").size()) == std::string("HTTP/1.")) {
        if ((substr = version.substr(std::string("HTTP/1.").size(), std::string::npos)) == std::string("0"))
            return (true);
        else {
            for (const char& c: substr) {
                if (!(c >= '0' && c <= '9'))
                    return (false);
            }
            version = "HTTP/1.1";
            return (true);
        }
    }
    return (false);
}

apouche::Method HttpParser::_getMethodFromName(std::string const& name)
{
    if (HttpParser::m_methodsFromName.find(name) == HttpParser::m_methodsFromName.end())
        return (apouche::Method::UndefinedRequestMethod);
    return (HttpParser::m_methodsFromName.at(name));
}

std::stringstream &HttpParser::getline(std::stringstream &ss,
                                        std::string &line,
                                        std::string const& delimiter,
                                        bool &toeof)
{
    size_t i = 0;
    char *eatableDelimiters = new char[delimiter.size()];
    std::streambuf* sb = ss.rdbuf();

    line.clear();
    while (true) {
        int c = sb->sgetc();

        if (c == EOF) {
            if (toeof) {
                sb->sbumpc();
                if (line.empty())
                    ss.setstate(std::ios::eofbit);
                break;
            }
            else {
                for (i = 0; i < line.size(); ++i) {
                    sb->sungetc();
                }
                toeof = true;
                line.clear();
                break;
            }
        }
        else if (delimiter.size() > 0 && c == delimiter.at(0)) {
            size_t sz;

            if ((sz = sb->sgetn(eatableDelimiters, delimiter.size())) == delimiter.size() &&
                std::string(eatableDelimiters, sz) == delimiter) {
                toeof = false;
                break;
            }
            else {
                for (i = 0; i < delimiter.size(); ++i) {
                    sb->sungetc();
                }
            }
        }
        line += (char)(sb->sbumpc());
    }
    delete[] eatableDelimiters;
    return ss;
}

bool HttpParser::parseStartLine(apouche::IHttpRequest *request, std::string &line) throw(HttpParserException)
{
    size_t pos;

    if ((pos = line.find(' ')) == std::string::npos) {
        throw HttpParserException("httpparser::method::no_space_after", apouche::StatusCode::BadRequest);
    }

    std::string method = line.substr(0, pos);
    line = line.substr(pos + 1, std::string::npos);

    if (method.empty())
        throw HttpParserException("httpparser::method::no_method", apouche::StatusCode::BadRequest);
    else if (HttpParser::_getMethodFromName(method) == apouche::Method::UndefinedRequestMethod)
        throw HttpParserException("httpparser::method::undefined_method", apouche::StatusCode::BadRequest);
    else
        request->setMethod(HttpParser::_getMethodFromName(method));

    if (line.empty()) {
        throw HttpParserException("httpparser::uri::empty", apouche::StatusCode::BadRequest);
    }

    if ((pos = line.find(' ')) == std::string::npos) {
        throw HttpParserException("httpparser::uri::no_space_after", apouche::StatusCode::BadRequest);
    }

    std::string request_uri = line.substr(0, pos);
    line = line.substr(pos + 1, std::string::npos);
    if (request_uri.empty())
        throw HttpParserException("httpparser::uri::no_uri", apouche::StatusCode::BadRequest);
    else if (request_uri.size() > 8192)
        throw HttpParserException("httpparser::uri::uri_too_long", apouche::StatusCode::RequestURITooLarge);
    else if (!HttpParser::checkUri(request_uri))
        throw HttpParserException("httpparser::uri::invalid_uri", apouche::StatusCode::BadRequest);
    else
        request->setURI(request_uri);

    if ((pos = request_uri.find("?")) != std::string::npos) {
        std::map<std::string, std::string> uriParameters;
        std::string parameters = request_uri.substr(pos + 1, std::string::npos);

        while (!parameters.empty()) {
            size_t endpos;
            std::string key;
            std::string value;

            endpos = parameters.find('&');
            if ((pos = parameters.find('=')) != std::string::npos) {
                key = parameters.substr(0, pos);

                value = parameters.substr(pos + 1, endpos);
            }
            else {
                key = parameters.substr(0, endpos);
            }
            uriParameters[key] = value;

            if (endpos != std::string::npos)
                parameters = parameters.substr(endpos + 1, std::string::npos);
            else
                parameters = "";
        }
        request->setUriParameters(uriParameters);
    }

    if (line.empty()) {
        throw HttpParserException("httpparser::http_version::no_http_version", apouche::StatusCode::BadRequest);
        return (false);
    }

    std::string http_version = line;
    if (!HttpParser::checkHttpVersion(http_version))
        throw HttpParserException("httpparser::http_version::invalid_http_version", apouche::StatusCode::BadRequest);
    else
        request->setVersion(http_version);
    return (true);
}

bool HttpParser::parseRequest(apouche::IHttpRequest *request, std::string const& entry) throw(HttpParserException)
{
    bool toeof = false;
    std::string line;
    std::stringstream ss(entry);

    this->m_logger.info("Parsing HTTP request...");
    if (request == nullptr)
        throw HttpParserException("httpparser::httprequest::null", apouche::StatusCode::BadRequest);

    HttpParser::getline(ss, line, "\r\n", toeof);
    if (line.empty()) {
        if (toeof == false)
            throw HttpParserException("httpparser::startline::empty", apouche::StatusCode::BadRequest);
        else
            return (false);
    }

    this->m_logger.info("Parsing start line...");
    if (!HttpParser::parseStartLine(request, line)) {
        return (false);
    }
    this->m_logger.info("Parsing start line done.");

    this->m_logger.info("Parsing headers...");
    std::map<std::string, std::string> map;
    apouche::IHttpHeader *header = request->getHeaders();

    if (header == nullptr)
        throw HttpParserException("httpparser::getHeaders::null", apouche::StatusCode::InternalServerError);

    toeof = false;
    while (HttpParser::getline(ss, line, "\r\n", toeof) && !line.empty()) {
        std::string key;
        std::string value;

        this->m_logger.info("Parsing new header line...");
        if (!HttpParser::parseHeaderLine(line, key, value))
            throw HttpParserException("httpparser::header::invalid_header_field", apouche::StatusCode::BadRequest);
        std::cerr << key << ": " << value << std::endl;
        if (map.find(key) != map.end())
            std::cerr << "[WARNING] header `" << key
                      << "` already exists. Overwriting it." << std::endl;
        map[key] = value;
        this->m_logger.info("Parsing new header line done.");
        toeof = false;
    }
    header->setAllHeaders(map);
    this->m_logger.info("Parsing headers done.");

    if (toeof == true)
        return (false);

    std::string bodyContent;
    apouche::IHttpBody *body = request->getBody();

    this->m_logger.info("Parsing body...");
    if (body == nullptr)
        throw HttpParserException("httpparser::body::null", apouche::StatusCode::InternalServerError);

    toeof = true;
    HttpParser::getline(ss, bodyContent, "", toeof);

    size_t content_length;
    try {
        std::string header = request->getHeaders()->getHeader("content-length");
        content_length = stoi(header);
    }
    catch (std::out_of_range const& e) {
        content_length = 0;
    }

    if (bodyContent.size() > content_length)
        bodyContent = bodyContent.substr(0, content_length);
    body->setBody(bodyContent);
    this->m_logger.info("Parsing body done.");

    this->m_logger.info("Parsing HTTP request done.");
    return (true);
}

inline char   *buffCopy(char *out, const char *in, size_t size)
{
  memcpy(out, in, size);
  return (&(out[size]));
}

Buffer::Data    *HttpParser::toData(apouche::IHttpResponse const *response)
{
  std::uint64_t totalSize = 0;
  std::string   status = std::to_string(response->getStatus());
  const std::map<std::string, std::string>  &headers = response->getHeaders()->getAllHeader();
  Buffer::Data  *buffer;
  char          *ss;

  totalSize += response->getVersion().size() + status.size() + response->getStatusDescription().size() + 4;
  for (auto ite = headers.begin(); ite != headers.end(); ite++)
    totalSize += ite->first.size() + ite->second.size() + 4;
  totalSize += 2 + response->getBody()->getBody().size();
  buffer = Buffer::createData(totalSize);
  ss = buffer->data;
  ss = buffCopy(ss, response->getVersion().c_str(), response->getVersion().size());
  ss = buffCopy(ss, " ", 1);
  ss = buffCopy(ss, status.c_str(), status.size());
  ss = buffCopy(ss, " ", 1);
  ss = buffCopy(ss, response->getStatusDescription().c_str(), response->getStatusDescription().size());
  ss = buffCopy(ss, "\r\n", 2);
  for (auto ite = headers.begin(); ite != headers.end(); ite++)
  {
    ss = buffCopy(ss, ite->first.c_str(), ite->first.size());
    ss = buffCopy(ss, ": ", 2);
    ss = buffCopy(ss, ite->second.c_str(), ite->second.size());
    ss = buffCopy(ss, "\r\n", 2);
  }
  ss = buffCopy(ss, "\r\n", 2);
  ss = buffCopy(ss, response->getBody()->getBody().c_str(), response->getBody()->getBody().size());
  return (buffer);
}

const std::map<std::string, apouche::Method> HttpParser::m_methodsFromName = {
    {"GET", apouche::Method::Get},
    {"POST", apouche::Method::Post},
    {"OPTIONS", apouche::Method::Options},
    {"HEAD", apouche::Method::Head},
    {"PUT", apouche::Method::Put},
    {"DELETE", apouche::Method::Delete},
    {"TRACE", apouche::Method::Trace},
    {"CONNECT", apouche::Method::Connect},
    {"UNDEFINED", apouche::Method::UndefinedRequestMethod},
};
