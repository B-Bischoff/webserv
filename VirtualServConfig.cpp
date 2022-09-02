#include "VirtualServConfig.hpp"

VirtualServConfig::VirtualServConfig() : _client_max_body_size(0), _autoIndex(false)
{

}

VirtualServConfig::~VirtualServConfig()
{
	
}

void	VirtualServConfig::setListenIp(std::string ip)
{
	_vServ["listen"].push_back(ip);
}

std::string	VirtualServConfig::getListenIp(void) const
{
	return (_vServ.at("listen")[0]);
}

void	VirtualServConfig::setListenPort(std::string port)
{
	_vServ["listen"].push_back(port);
}

std::string	VirtualServConfig::getListenPort(void) const
{
	return (_vServ.at("listen")[1]);
}

void	VirtualServConfig::setServerName(std::vector<std::string> serverName)
{
	_vServ["server_name"] = serverName;
}

std::vector<std::string>	VirtualServConfig::getServerName(void) const
{
	return (_vServ.at("server_name"));
}

void	VirtualServConfig::setRoot(std::string path)
{
	_vServ["root"].push_back(path);
}

std::string	VirtualServConfig::getRoot(void) const
{
	return (_vServ.at("root")[0]);
}

void	VirtualServConfig::setIndex(std::string path)
{
	_vServ["index"].push_back(path);
}

std::string	VirtualServConfig::getIndex(void) const
{
	return (_vServ.at("index")[0]);
}

void	VirtualServConfig::setAccessLog(std::vector<std::string> accessLog)
{
	_vServ["access_log"] = accessLog;
}

std::vector<std::string>	VirtualServConfig::getAccessLog(void) const
{
	return (_vServ.at("access_log"));
}

void	VirtualServConfig::setErrorLog(std::vector<std::string> errorLog)
{
	_vServ["error_Log"] = errorLog;
}

std::vector<std::string>	VirtualServConfig::getErrorLog(void) const
{
	return (_vServ.at("error_Log"));
}

void	VirtualServConfig::setMethodGet(std::string method)
{
	_vServ["method"].reserve(1);
	_vServ["method"][0] = method;
}

std::string	VirtualServConfig::getMethodGet(void) const
{
	return (_vServ.at("method")[0]);
}

void	VirtualServConfig::setMethodPost(std::string method)
{
	_vServ["method"].reserve(1);
	_vServ["method"][1] = method;
}

std::string	VirtualServConfig::getMethodPost(void) const
{
	return (_vServ.at("method")[1]);
}

void	VirtualServConfig::setMethodDelete(std::string method)
{
	_vServ["method"].reserve(1);
	_vServ["method"][2] = method;
}

std::string	VirtualServConfig::getMethodDelete(void) const
{
	return (_vServ.at("method")[2]);
}