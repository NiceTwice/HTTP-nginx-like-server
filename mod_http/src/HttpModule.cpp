//
// HttpModule.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Tue Jan 31 11:16:41 2017 Adrien Vasseur
// Last update Tue Jan 31 11:16:41 2017 Adrien Vasseur
//

#include    "HttpModule.hpp"

HttpModule::HttpModule(apouche::Logger *log) : AModule("mod_http",
                                                       "Main http module",
                                                       "1.0.0", log)
{
  this->_logger->debug("mod_http -> Constructor!");
}

HttpModule::~HttpModule()
{
  this->_logger->debug("mod_http -> Destructor!");
}

void        HttpModule::registerEvents(apouche::EventHandler *handler)
{
  auto callback = std::bind(&HttpModule::checkRequest, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
  apouche::Event<apouche::ReturnState (apouche::IHttpRequest *, apouche::IHttpResponse *, apouche::IHttpConf *)>  event("Request preparator", apouche::Weight::LOWEST, callback);
  handler->_afterCreateResponse.addEvent(event);
}

apouche::ReturnState        HttpModule::checkRequest(apouche::IHttpRequest *, apouche::IHttpResponse *response, apouche::IHttpConf *)
{
  //if (response->getBody()->getBody().size() == 0)
  //  response->getBody()->setBody("<h1>T'es con putain!</h1>\n<h2>C'est pas compliqué d'envoyer une requête valide...!</h2>");
  //response->getHeaders()->setHeader("Content-Type", "text/html; charset=UTF-8");
  response->getHeaders()->setHeader("Content-Length", std::to_string(response->getBody()->getBody().size()));
  response->getHeaders()->setHeader("Connection", "Close");
  this->_logger->debug("Checking request parsing...");
  return (apouche::SUCCESS);
}