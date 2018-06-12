//
// HttpRequest.cpp for ZIA in /home/cugede/work/Epitech/rendu/TEK3/ZIA/cpp_zia/core/src/api/HttpRequest
//
// Made by Quentin Guerin
// Login <quentin.guerin@epitech.net>
//
// Started on  Tue Jan 31 16:48:42 2017 Quentin Guerin
// Last update Tue Jan 31 16:55:11 2017 Quentin Guerin
//

#include <sstream>
#include <iostream>
#include "api/HttpRequest/HttpRequest.hh"

apouche::HttpRequest::~HttpRequest() {
  delete m_header;
  delete m_body;
}

apouche::Method apouche::HttpRequest::getMethod() const {
    return m_method;
}

void apouche::HttpRequest::setMethod(apouche::Method method) {
    m_method = method;
}

const std::string &apouche::HttpRequest::getURI() const {
    return m_uri;
}

void apouche::HttpRequest::setURI(const std::string &URI) {
    m_uri = URI;
}

bool apouche::HttpRequest::containsUriParameters(const std::string &key) const {
    return !(m_uriParameters.find(key) == m_uriParameters.end());
}

const std::map<std::string, std::string> &apouche::HttpRequest::getUriParameters() const {
    return m_uriParameters;
}

const std::string &apouche::HttpRequest::getUriParameter(const std::string &key) const {
    return m_uriParameters.at(key);
}

void apouche::HttpRequest::setUriParameter(const std::string &key, const std::string &value) {
    m_uriParameters[key] = value;
}

const std::string apouche::HttpRequest::getRequestLine() {
    return m_mapMethodFromEnum.at(m_method) + " " + m_uri + " " + getVersion();
}

void apouche::HttpRequest::setUriParameters(const std::map<std::string, std::string> &uriParameters) {
    m_uriParameters = uriParameters;
}

apouche::IHttpHeader *apouche::HttpRequest::getHeaders() {
    return m_header;
}

const apouche::IHttpHeader *apouche::HttpRequest::getHeaders() const {
    return m_header;
}

void apouche::HttpRequest::setHeaders(IHttpHeader *header) {
    delete m_header;
    m_header = header;
}

apouche::IHttpBody *apouche::HttpRequest::getBody() {
    return m_body;
}

const apouche::IHttpBody *apouche::HttpRequest::getBody() const {
  return m_body;
}

void apouche::HttpRequest::setBody(IHttpBody *body) {
    delete m_body;
    m_body = body;
}

const std::string &apouche::HttpRequest::getVersion() const {
    return m_version;
}

void apouche::HttpRequest::setVersion(const std::string &version) {
    m_version = version;
}

apouche::HttpRequest::HttpRequest():
    m_header(new apouche::HttpHeader),
    m_body(new apouche::HttpBody),
    m_method(apouche::Method::UndefinedRequestMethod)
{
}

const std::map <apouche::Method,  std::string> apouche::HttpRequest::m_mapMethodFromEnum = {
    {Method::Post, "POST"},
    {Method::Get, "GET"},
    {Method::Options, "OPTIONS"},
    {Method::Head, "HEAD"},
    {Method::Put, "PUT"},
    {Method::Delete, "DELETE"},
    {Method::Trace, "TRACE"},
    {Method::Connect, "CONNECT"},
    {Method::UndefinedRequestMethod, "UNDEFINED"}
};