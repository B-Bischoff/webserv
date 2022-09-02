#include "VirtualServerConfig.hpp"

VirtualServerConfig::VirtualServerConfig() : _client_max_body_size(0), _autoIndex(false)
{

}

VirtualServerConfig::~VirtualServerConfig()
{
	
}

void	VirtualServerConfig::setListenIp(std::string ip)
{
	_vServ["listen"].push_back(ip);
}

const std::string&	VirtualServerConfig::getIp(void) const
{
	return (_vServ.at("listen")[0]);
}

void	VirtualServerConfig::setListenPort(std::string port)
{
	_vServ["listen"].push_back(port);
}

unsigned int	VirtualServerConfig::getPort(void) const
{
	return (std::stoi(_vServ.at("listen")[1], NULL, 10));
}

void	VirtualServerConfig::setServerName(std::vector<std::string> serverName)
{
	_vServ["server_name"] = serverName;
}

const std::vector<std::string>&	VirtualServerConfig::getServerName(void) const
{
	return (_vServ.at("server_name"));
}

void	VirtualServerConfig::setRoot(std::string path)
{
	_vServ["root"].push_back(path);
}

std::string	VirtualServerConfig::getRoot(void) const
{
	return (_vServ.at("root")[0]);
}

void	VirtualServerConfig::setIndex(std::string path)
{
	_vServ["index"].push_back(path);
}

std::string	VirtualServerConfig::getIndex(void) const
{
	return (_vServ.at("index")[0]);
}

void	VirtualServerConfig::setAccessLog(std::vector<std::string> accessLog)
{
	_vServ["access_log"] = accessLog;
}

std::vector<std::string>	VirtualServerConfig::getAccessLog(void) const
{
	return (_vServ.at("access_log"));
}

void	VirtualServerConfig::setErrorLog(std::vector<std::string> errorLog)
{
	_vServ["error_Log"] = errorLog;
}

std::vector<std::string>	VirtualServerConfig::getErrorLog(void) const
{
	return (_vServ.at("error_Log"));
}

void	VirtualServerConfig::setMethodGet(std::string method)
{
	_vServ["method"].reserve(1);
	_vServ["method"][0] = method;
}

std::string	VirtualServerConfig::getMethodGet(void) const
{
	return (_vServ.at("method")[0]);
}

void	VirtualServerConfig::setMethodPost(std::string method)
{
	_vServ["method"].reserve(1);
	_vServ["method"][1] = method;
}

std::string	VirtualServerConfig::getMethodPost(void) const
{
	return (_vServ.at("method")[1]);
}

void	VirtualServerConfig::setMethodDelete(std::string method)
{
	_vServ["method"].reserve(1);
	_vServ["method"][2] = method;
}

std::string	VirtualServerConfig::getMethodDelete(void) const
{
	return (_vServ.at("method")[2]);
}
