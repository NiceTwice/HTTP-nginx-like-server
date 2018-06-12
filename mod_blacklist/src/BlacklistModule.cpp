//
// BlacklistModule.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sun Feb 05 21:47:34 2017 Adrien Vasseur
// Last update Sun Feb 05 21:47:34 2017 Adrien Vasseur
//

#include    <fstream>
#include    "BlacklistModule.hpp"

BlacklistModule::BlacklistModule(apouche::Logger *log) : AModule("mod_blacklist",
                                                                 "Block ip address module",
                                                                 "1.0.0", log)
{
  this->_loaded = false;
  this->_logger->debug("mod_blacklist -> Constructor!");
}

BlacklistModule::~BlacklistModule()
{
  this->_logger->debug("mod_blacklist -> Destructor!");
}

bool        BlacklistModule::loadFile(const std::string &filename)
{
  std::ifstream file;
  std::string   line;

  this->_loaded = true;
  file.open(filename, std::ios::in);
  if (file.is_open())
  {
    while (std::getline(file, line))
    {
      if (line.size() > 0)
        this->_blacklisted[line] = 1;
    }
  }
  else
    this->_logger->error("Cannot open file: " + filename);
}

bool        BlacklistModule::isBlacklisted(const std::string &ip)
{
  std::map<std::string, int>::iterator  found = this->_blacklisted.find(ip);

  return (found != this->_blacklisted.end());
}

void        BlacklistModule::registerEvents(apouche::EventHandler *handler)
{
  auto callback = std::bind(&BlacklistModule::onConnect, this, std::placeholders::_1, std::placeholders::_2);
  apouche::Event<apouche::ReturnState (apouche::IZiaConnection *, apouche::IHttpConf *)>  event("On connect event", apouche::Weight::HIGHEST, callback);
  handler->_afterConnect.addEvent(event);
}

apouche::ReturnState  BlacklistModule::onConnect(apouche::IZiaConnection *connection, apouche::IHttpConf *config)
{
  const std::string   *fileConf;
  std::string         filename;

  if (!this->_loaded)
  {
    filename = "./blacklisted.txt";
    if (config != NULL && (fileConf = config->getConf("mod_parameters.blacklist.file")) != NULL)
      filename = *fileConf;
    this->loadFile(filename);
  }
  if (this->isBlacklisted(connection->getNativeHandle().getRemoteAddress()))
    return (apouche::FATAL_ERROR);
  return (apouche::SUCCESS);
}