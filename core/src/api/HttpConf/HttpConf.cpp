//
// HttpConf.cpp for zia in /home/blazer/Epitech/Rendu/cpp_zia/src/loader/
//
// Made by Tom CZAYKA
// Login        <tom.czayka@epitech.eu>
//
// Started on   mer. oct.       19 12:49:58 2016 tom.czayka
// Last update  mer. oct.       19 12:49:58 2016 tom.czayka
//

#include <fstream>
#include "api/HttpConf/HttpConf.hpp"

HttpConf::HttpConf() : _path("conf/config.json"), _isReady(false)
{
	this->_defaultConf = false;
}

HttpConf::HttpConf(const std::map<std::string, std::string> &cpy) : _parameters(cpy)
{
}

HttpConf::~HttpConf()
{
	this->_parameters.clear();
}

const	std::map<std::string, std::string> &HttpConf::getAllConf() const
{
	return (this->_parameters);
}

void	HttpConf::setAllConf(const std::map<std::string, std::string> &parameters)
{
	this->_parameters = parameters;
}

const	std::string *HttpConf::getConf(const std::string &key) const
{
	std::map<std::string, std::string>::const_iterator ite;

	if ((ite = this->_parameters.find(key)) != this->_parameters.end())
		return (&(ite->second));
	return (NULL);
}

void	HttpConf::setConf(const std::string &key, const std::string &value)
{
	this->_addValue(key, value);
}

void	HttpConf::deleteConf(const std::string &key)
{
	std::map<std::string, std::string>::iterator ite;

	if ((ite = this->_parameters.find(key)) != this->_parameters.end())
		this->_parameters.erase(ite);
}

bool	HttpConf::containsConf(const std::string &) const
{
	return (this->_isReady);
}

void	HttpConf::_printMap()
{
	for (std::map<std::string, std::string>::iterator it = this->_parameters.begin(); it != this->_parameters.end(); ++it)
	{
    	std::cout << "[" << it->first << "] = " << it->second << std::endl;
    }
}

void	HttpConf::_addValue(const std::string &key, const std::string &value)
{
	this->_parameters[key] = value;
}

void	HttpConf::_parseParameters(const std::string &port, const JSONArray &array)
{
	size_t size;
	std::string parameters;

	size = array.size();
	for (size_t i = 0; i < size; i++)
	{
		if (i != 0)
			parameters += ",";
		parameters += array.getAt(i).getObject("value").getString("key");
		this->_addValue("server.port." + port + ".parameters." + array.getAt(i).getObject("value").getString("key"), array.getAt(i).getObject("value").getString("value"));
	}
	this->_addValue("server.port." + port + ".parameters", parameters);
}

void	HttpConf::_parseServerInfos(const JSONObject &root)
{
	size_t size;
	std::string actualPort;
	std::string ports;

	size = root.getArray("server").size();
	ports = "";
	for (size_t i = 0; i < size; i++)
	{
		if (i != 0)
			ports += ",";
		actualPort = std::to_string(root.getArray("server").getAt(i).getObject("value").getInteger("port"));
		ports += actualPort;
		this->_addValue("server.port." + actualPort + ".ssl", root.getArray("server").getAt(i).getObject("value").getString("ssl"));
		this->_parseParameters(actualPort, root.getArray("server").getAt(i).getObject("value").getArray("parameters"));
	}
	this->_addValue("server.port", ports);
}

void	HttpConf::_parseModules(const JSONObject &root)
{
	size_t size;
	std::string modules;

	size = root.getArray("modules").size();
	modules = "";
	for (size_t i = 0; i < size; i++)
	{
		if (i != 0)
			modules += ",";
		modules += root.getArray("modules").getAt(i).getObject("value").getString("value");
	}
	this->_addValue("modules", modules);
}

void	HttpConf::_parseOwner(const JSONObject &root)
{
	this->_addValue("owner.user", root.getObject("owner").getString("user"));
	this->_addValue("owner.group", root.getObject("owner").getString("group"));
}

void	HttpConf::_parseDocumentRoot(const JSONObject &root)
{
	this->_addValue("document_root.path", root.getObject("document_root").getString("path"));
	this->_addValue("document_root.index_directory", root.getObject("document_root").getString("index_directory"));
}

void	HttpConf::_parseDirectory(const JSONObject &root)
{
	size_t size;
	std::string entities;
	std::string actualEntity;

	size = root.getObject("directory").getObject("require").getArray("entities").size();
	entities = "";
	this->_addValue("directory.path", root.getObject("directory").getString("path"));
	this->_addValue("directory.require.not", root.getObject("directory").getObject("require").getString("not"));
	for (size_t i = 0; i < size; i++)
	{
		if (i != 0)
			entities += ",";
		actualEntity = std::to_string(root.getObject("directory").getObject("require").getArray("entities").getAt(i).getObject("value").getInteger("id"));
		entities += actualEntity;
		this->_addValue("directory.require.entities." + actualEntity + ".name", root.getObject("directory").getObject("require").getArray("entities").getAt(i).getObject("value").getString("name"));
		this->_addValue("directory.require.entities." + actualEntity + ".value", root.getObject("directory").getObject("require").getArray("entities").getAt(i).getObject("value").getString("value"));
	}
	this->_addValue("directory.require.entities", entities);
}

void	HttpConf::_parseModuleParameters(const JSONObject &root)
{
	size_t size;

	size = root.getArray("mod_parameters").size();
	for (size_t i = 0; i < size; i++)
	{
		this->_addValue("mod_parameters." + root.getArray("mod_parameters").getAt(i).getObject("value").getString("key"), root.getArray("mod_parameters").getAt(i).getObject("value").getString("value"));
	}
}

bool	HttpConf::_setDefaultConfig()
{
	std::cout << "[WARNING] Running with default configuration" << std::endl;
	this->_addValue("server.port", "80,443");
	this->_addValue("server.port.80.ssl", "false");
	this->_addValue("server.port.443.ssl", "true");
	this->_addValue("document_root.path", "/var/www/html");
	this->_addValue("modules", "");
	return (this->_defaultConf = true);
}

bool	HttpConf::update()
{
	ZiaMD5 localHasher;

	if (!this->_defaultConf)
	{
		if (!this->_getFile(localHasher))
			return (false);
		if (this->_hasher != localHasher)
			if (this->_buildMap())
			{
				std::cout << "[CONFIG] Updating configuration file..." << std::endl;
				this->_hasher = localHasher;
				return (true);
			}
	}
	return (false);
}

bool	HttpConf::_getFile(ZiaMD5 &hasher)
{
	std::ifstream file(this->_path);
	std::string str;
	std::string file_contents;

	this->_parser.clear();
	if (file.is_open())
    {
    	while (std::getline(file, str))
    	{
    		this->_parser.feed(str + '\n');
    		hasher.feed(str);
    	}
    	hasher.calc();
    	return (true);
    }
    return (false);
}

bool	HttpConf::_buildMap()
{
	try
	{
		if (this->_parser.parse())
		{
			const JSONObject &root = this->_parser.getRoot();
			this->_parseServerInfos(root);
			this->_parseOwner(root);
			this->_parseDocumentRoot(root);
			this->_parseDirectory(root);
			this->_parseModules(root);
			this->_parseModuleParameters(root);
			return (this->_isReady = true);
		}
	}
	catch (JSONException &e)
	{

	}
	std::cerr << "[ERROR] Bad syntax in configuration file" << std::endl;
	return (this->_isReady = false);
}

bool	HttpConf::load()
{
	if (!this->_getFile(this->_hasher))
		return (this->_setDefaultConfig());
	std::cout << "[CONFIG] Running with configuration file: " << this->_path << std::endl;
	return (this->_buildMap());
}