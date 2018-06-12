//
// HttpConf.hpp for zia in /home/blazer/Epitech/Rendu/cpp_zia/include/api/HttpConf
//
// Made by Tom CZAYKA
// Login        <tom.czayka@epitech.eu>
//
// Started on   mer. oct.       19 12:49:58 2016 tom.czayka
// Last update  mer. oct.       19 12:49:58 2016 tom.czayka
//

#pragma once

#include "api/HttpConf/IHttpConf.hpp"
#include "json/JSONParser.hpp"
#include "json/JSONObject.hpp"
#include "json/JSONArray.hpp"
#include "json/JSONException.hpp"
#include "parser/ZiaMD5.hpp"

class HttpConf : public apouche::IHttpConf
{
public:
	HttpConf();
	HttpConf(const std::map<std::string, std::string> &);
	~HttpConf();

public:
	const	std::map<std::string, std::string> &getAllConf() const;
	void	setAllConf(const std::map<std::string, std::string> &);
	const	std::string *getConf(const std::string &) const;
	void	setConf(const std::string &, const std::string &);
	void	deleteConf(const std::string &);
	bool	containsConf(const std::string &) const;
	bool	update();
	bool	load();

private:
	void	_addValue(const std::string &, const std::string &);
	void	_parseParameters(const std::string &, const JSONArray &);
	void	_parseServerInfos(const JSONObject &);
	void	_parseOwner(const JSONObject &);
	void	_parseDocumentRoot(const JSONObject &);
	void	_parseDirectory(const JSONObject &);
	void	_parseModules(const JSONObject &);
	void	_parseModuleParameters(const JSONObject &);
	bool	_setDefaultConfig();
	bool	_getFile(ZiaMD5 &);
	bool	_buildMap();
	void	_printMap();

private:
	std::map<std::string, std::string> _parameters;
	ZiaMD5 _hasher;
	const unsigned char *signature;
	std::string _path;
	JSONParser _parser;
	bool _isReady;
	bool _defaultConf;
};