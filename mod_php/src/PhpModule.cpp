//
// PhpModule.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Tue Jan 31 11:16:41 2017 Adrien Vasseur
// Last update Fri Feb 03 16:30:47 2017 Quentin Guerin
//

#ifndef _WIN32
#include <sys/wait.h>
#include <unistd.h>
#else
#include <io.h>
#include <fcntl.h>
#include <process.h>
#include <Windows.h>
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#ifdef ERROR
#undef ERROR
#endif
#endif
#include <sstream>
#include <string.h>
#include "PhpModule.hpp"
#include "api/HttpResponse/HttpResponse.hh"
#include "parser/HttpParser.hh"

PhpModule::PhpModule(apouche::Logger *log) : AModule("mod_php",
  "php module",
  "1.0.0", log)
{
  this->_logger->debug("mod_php -> Constructor!");
}

PhpModule::~PhpModule()
{
  this->_logger->debug("mod_php -> Destructor!");
}

std::string PhpModule::_prepareCommand(apouche::IHttpRequest *request, apouche::IHttpConf *config, char ***env) const throw (PhpModuleException)
{
    size_t i = 0;
    std::stringstream command;
    std::map<std::string, std::string> const& uriParameters = request->getUriParameters();

    // Keep-Alive
    command << "HTTP_KEEP_ALIVE=";
    try {
        command << request->getHeaders()->getHeader("Keep-Alive");
    }
    catch (std::out_of_range const& e) {
    }
    command << std::endl;

    // Connection
    command << "HTTP_CONNECTION=";
    try {
        command << request->getHeaders()->getHeader("Connection");
    }
    catch (std::out_of_range const& e) {
    }
    command << std::endl;

    // Cookie
    command << "HTTP_COOKIE=";
    try {
        command << request->getHeaders()->getHeader("Cookie");
    }
    catch (std::out_of_range const& e) {
    }
    command << std::endl;

    // Cache-Control
    command << "HTTP_CACHE_CONTROL=";
    try {
        command << request->getHeaders()->getHeader("Cache-Control");
    }
    catch (std::out_of_range const& e) {
    }
    command << std::endl;

    // $_SERVER['SERVER_PROTOCOL']
    command << "SERVER_PROTOCOL=" << request->getVersion() << std::endl;

    // $_SERVER['CONTENT_TYPE']
    command << "CONTENT_TYPE=";
    try {
        command << request->getHeaders()->getHeader("Content-type");
    }
    catch (std::out_of_range const& e) {
        command << "text/html; charset=UTF-8";
    }
    command << std::endl;

    // $_SERVER['CONTENT_LENGTH']
    command << "CONTENT_LENGTH=";
    try {
        command << stoi(request->getHeaders()->getHeader("Content-length"));
    }
    catch (std::out_of_range const& e) {
        command << request->getBody()->getBody().size();
    }
    command << std::endl;

    // $_SERVER['SERVER_NAME']
    command << "SERVER_NAME=" << "Zia - v1.0.0" << std::endl;

    // $_SERVER['REQUEST_URI']
    command << "REQUEST_URI=" << request->getURI() << std::endl;

    const std::string *path;

    if ((path = config->getConf("document_root.path")) == NULL)
        throw PhpModuleException("phpmodule::config::no_document_root", apouche::StatusCode::InternalServerError);

    // $_SERVER['SCRIPT_FILENAME']
    command << "SCRIPT_FILENAME=" << (path->back() == '/' ? path->substr(0, path->size() - 1) : (*path));
    command << request->getURI().substr(0, request->getURI().find("?")) << std::endl;

    // $_SERVER['REDIRECT_STATUS']
    command << "REDIRECT_STATUS=true" << std::endl;

    // $_SERVER['GATEWAY_INTERFACE']
    command << "GATEWAY_INTERFACE=CGI/1.1" << std::endl;

    // $_SERVER['REQUEST_METHOD']
    command << "REQUEST_METHOD=";
    if ((this->_methodAsString.find(request->getMethod())) != this->_methodAsString.end())
        command << this->_methodAsString.at(request->getMethod());
    command << std::endl;

    // $_SERVER['QUERY_STRING']
    i = 0;
    command << "QUERY_STRING=";
    for (auto const& pair: uriParameters) {
        command << pair.first + "=" + pair.second;
        if (++i < uriParameters.size())
            command << '&';
    }
    command << std::endl;

    // Convert std::stringstream to std::vector<std::string>
    i = 0;
    std::string line;
    std::vector<std::string> envVector;

    while (std::getline(command, line, '\n') && !line.empty()) {
        envVector.push_back(line);
    }
    command.str("");

    // Convert std::vector<std::string> to char **
    *env = new char*[envVector.size() + 1];
    for (std::string const& v: envVector) {
        (*env)[i] = new char[v.size() + 1];

        strcpy((*env)[i], v.c_str());
        i++;
    }
    (*env)[i] = NULL;

    // $_GET
    for (auto const& pair: uriParameters) {
        command << " '" + pair.first + "=" + pair.second + "'";
    }

    std::cerr << command.str() << std::endl;
    return (command.str());
}


void PhpModule::registerEvents(apouche::EventHandler *handler)
{
  auto callback = std::bind(&PhpModule::execute, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
  apouche::Event<apouche::ReturnState(apouche::IHttpRequest *, apouche::IHttpResponse *, apouche::IHttpConf *)>  event("PHP executor", apouche::Weight::HIGH, callback);
  handler->_afterCreateResponse.addEvent(event);
}

apouche::ReturnState PhpModule::execute(apouche::IHttpRequest *request, apouche::IHttpResponse *response, apouche::IHttpConf *config)
{
    size_t i;
    FILE *in;
    FILE *out;
    FILE *err;
#ifndef _WIN32
    pid_t pid;
#else
    int pid;
#endif
    int fd[6];
    char **env;
    std::string uri;
    std::string gets;
    std::string command;
    std::string extension;
    char *argv[4] = {NULL, NULL, NULL, NULL};

    uri = request->getURI().substr(0, request->getURI().find("?"));

    if (uri.size() > 4)
        extension = uri.substr(uri.size() - 4, std::string::npos);

    if (((apouche::HttpResponse *)response)->getCategory() == apouche::Category::ClientError ||
        !(extension == ".php" || extension == ".php3" ||
          extension == ".php5" || extension == ".php7"))
        return (apouche::SUCCESS);

#ifndef _WIN32
    pipe(&fd[0]);
    pipe(&fd[2]);
    pipe(&fd[4]);
#else
    _pipe(&fd[0], 512, _O_BINARY);
    _pipe(&fd[2], 512, _O_BINARY);
    _pipe(&fd[4], 512, _O_BINARY);
#endif

    try {
        gets = this->_prepareCommand(request, config, &env);
    }
    catch (PhpModuleException const& e) {
        response->setStatus(e.code());
        return (apouche::SUCCESS);
    }

#ifndef _WIN32
    switch (pid = fork()) {
#else
    char hstr[20];
    char path[256];
    GetModuleFileName(NULL, path, 256);
    switch (pid = _spawnl(P_NOWAIT, path, path, hstr, NULL)) {
#endif
        case -1:
            perror("unable to fork()");
            break ;

        case 0:
#ifndef _WIN32
            close(fd[1]);   // Close write end of stdin.
            close(fd[2]);   // Close read end of stdout.
            close(fd[4]);   // Close read end of stderr.
            dup2(fd[0], STDIN_FILENO);  // Have stdin read from the first pipe.
            dup2(fd[3], STDOUT_FILENO); // Have stdout write to the second pipe.
            dup2(fd[5], STDERR_FILENO); // Have stderr write to the third pipe.
            argv[0] = strdup("/bin/php-cgi");
#else
            _close(fd[1]);   // Close write end of stdin.
            _close(fd[2]);   // Close read end of stdout.
            _close(fd[4]);   // Close read end of stderr.
            _dup2(fd[0], 0);  // Have stdin read from the first pipe.
            _dup2(fd[3], 1); // Have stdout write to the second pipe.
            _dup2(fd[5], 2); // Have stderr write to the third pipe.
            argv[0] = strdup("php-cgi.exe");
#endif
            
            argv[1] = strdup("--");
            if (gets.size() > 0)
                argv[2] = strdup(gets.c_str());
#ifndef _WIN32
            dup2(fd[0], STDIN_FILENO);  // Have stdin read from the first pipe.
            dup2(fd[3], STDOUT_FILENO); // Have stdout write to the second pipe.
            dup2(fd[5], STDERR_FILENO); // Have stderr write to the third pipe.
            execve("/bin/php-cgi", argv, env);
#else
            _dup2(fd[0], STDIN_FILENO);  // Have stdin read from the first pipe.
            _dup2(fd[3], STDOUT_FILENO); // Have stdout write to the second pipe.
            _dup2(fd[5], STDERR_FILENO); // Have stderr write to the third pipe.
            _execve("php-cgi.exe", argv, env);
#endif
            perror("execve() failed");
            _exit(1);

        default:
#ifndef _WIN32
            close(fd[0]); // Close read end of stdin.
            close(fd[3]); // Close write end of stdout.
            close(fd[5]); // Close write end of stderr.

            in = fdopen(fd[1], "wb");
            out = fdopen(fd[2], "rb");
            err = fdopen(fd[4], "rb");
#else
            _close(fd[0]); // Close read end of stdin.
            _close(fd[3]); // Close write end of stdout.
            _close(fd[5]); // Close write end of stderr.

            in = _fdopen(fd[1], "wb");
            out = _fdopen(fd[2], "rb");
            err = _fdopen(fd[4], "rb");
#endif

            if (in == NULL || out == NULL || err == NULL) {
                if (in != NULL) {
                    fclose(in);
                    if (out != NULL)
                        fclose(out);
                }
                response->setStatus(apouche::StatusCode::InternalServerError);
                return (apouche::SUCCESS);
            }

            // $_POST
            if (request->getMethod() == apouche::Method::Post)
                fwrite(request->getBody()->getBody().c_str(), sizeof(char), request->getBody()->getBody().size(), in);
            fclose(in);

            size_t size;
            bool done = false;
            char buffer[4096];
            std::string line;
            bool toeof = false;
            std::stringstream content;

            // Read PHP-CGI output
            while (!done && (size = fread(buffer, sizeof(char), 4096, out)) > 0) {
                content << std::string(buffer, size);
                done = size < 4096;
            }
            fclose(out);

            // Find headers
            while (HttpParser::getline(content, line, "\r\n", toeof) &&
                   !line.empty()) {
                std::string key;
                std::string value;

                // If header is invalid
                if (!HttpParser::parseHeaderLine(line, key, value))
                    return (apouche::ERROR);

                // Set new header
                response->getHeaders()->setHeader(key, value);
            }

            // Set remaining content as body
            toeof = true;
            HttpParser::getline(content, line, "", toeof);

            response->getBody()->setBody(line);

            fclose(err);
            response->setStatus(apouche::StatusCode::OK);

            i = 0;
            while (env[i]) {
                delete[] env[i];
                i++;
            }
            delete[] env;
    }
    return (apouche::SUCCESS);
}

const std::map<apouche::Method, std::string> PhpModule::_methodAsString = {
    {apouche::Method::Get, "GET"},
    {apouche::Method::Options, "OPTIONS"},
    {apouche::Method::Head, "HEAD"},
    {apouche::Method::Post, "POST"},
    {apouche::Method::Put, "PUT"},
    {apouche::Method::Delete, "DELETE"},
    {apouche::Method::Connect, "CONNECT"},
    {apouche::Method::Trace, "TRACE"}
};