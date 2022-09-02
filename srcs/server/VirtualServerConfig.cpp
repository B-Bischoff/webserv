#include "VirtualServerConfig.hpp"

VirtualServerConfig::VirtualServerConfig() : _client_max_body_size(1000000), _autoIndex(false)
{
	_vServ["server_name"].push_back("default");
	_vServ["access_log"].push_back("");
	_vServ["error_log"].push_back("");
	_vServ["listen"].push_back("0.0.0.0");
	_vServ["root"].push_back("/Users/tarchimb/42/webserv/webserv.cpp");
	_vServ["index"].push_back("");
	_vServ["listen"].push_back("8080");
	_vServ["method"].push_back("GET");
	_vServ["method"].push_back("");
	_vServ["method"].push_back("");
	_vServ["return"].push_back("");
}

VirtualServerConfig::~VirtualServerConfig()
{
	
}

void	VirtualServerConfig::setIp(std::string ip)
{
	_vServ["listen"][0] = ip;
}

const std::string&	VirtualServerConfig::getIp(void) const
{
	return (_vServ.at("listen")[0]);
}

void	VirtualServerConfig::setPort(std::string port)
{
	_vServ["listen"][1] = port;
}

unsigned int	VirtualServerConfig::getPort(void) const
{
	return std::stoi(_vServ.at("listen")[1], NULL, 10);
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
	_vServ["root"][0] = path;
}

std::string	VirtualServerConfig::getRoot(void) const
{
	return (_vServ.at("root")[0]);
}

void	VirtualServerConfig::setIndex(std::string path)
{
	_vServ["index"][0] = path;
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
	_vServ["error_log"] = errorLog;
}

std::vector<std::string>	VirtualServerConfig::getErrorLog(void) const
{
	return (_vServ.at("error_log"));
}

void	VirtualServerConfig::setMethodGet(std::string method)
{
	_vServ["method"][0] = method;
}

std::string	VirtualServerConfig::getMethodGet(void) const
{
	return (_vServ.at("method")[0]);
}

void	VirtualServerConfig::setMethodPost(std::string method)
{
	_vServ["method"][1] = method;
}

std::string	VirtualServerConfig::getMethodPost(void) const
{
	return (_vServ.at("method")[1]);
}

void	VirtualServerConfig::setMethodDelete(std::string method)
{
	_vServ["method"][2] = method;
}

std::string	VirtualServerConfig::getMethodDelete(void) const
{
	return (_vServ.at("method")[2]);
}

void	VirtualServerConfig::setAutoIndex(bool value)
{
	_autoIndex = value;
}

bool	VirtualServerConfig::getAutoIndex(void) const
{
	return (_autoIndex);
}

void	VirtualServerConfig::setReturn(std::vector<std::string> value)
{
	_vServ["return"] = value;
}

std::vector<std::string>	VirtualServerConfig::getReturn(void) const
{
	return (_vServ.at("return"));
}

void	VirtualServerConfig::setMaxBodySize(unsigned int size)
{
	_client_max_body_size = size;
}

unsigned int	VirtualServerConfig::getMaxBodySize(void) const
{
	return (_client_max_body_size);
}

void	VirtualServerConfig::setAutoIndexLoc(bool value, int locationBlock)
{
	_location[locationBlock]["autoindex"].push_back(std::to_string(value));
}

bool	VirtualServerConfig::getAutoIndexLoc(int locationBlock) const
{
	return ((_location[locationBlock].at("autoindex").at(0) == "off" ? true : false));
}

void	VirtualServerConfig::setReturnLoc(std::vector<std::string> value, int locationBlock)
{
	_location[locationBlock]["return"] = value;
}

std::vector<std::string>	VirtualServerConfig::getReturnLoc(int locationBlock) const
{
	return (_location[locationBlock].at("return"));
}


void	VirtualServerConfig::setRootLoc(std::string path, int locationBlock)
{
	_location[locationBlock]["root"][0] = path;
}

std::string	VirtualServerConfig::getRootLoc(int locationBlock) const
{
	return (_location[locationBlock].at("root")[0]);
}

void	VirtualServerConfig::setIndexLoc(std::string path, int locationBlock)
{
	_location[locationBlock]["index"][0] = path;
}

std::string	VirtualServerConfig::getIndexLoc(int locationBlock) const
{
	return (_location[locationBlock].at("index")[0]);
}

void	VirtualServerConfig::setMethodGetLoc(std::string method, int locationBlock)
{
	_location[locationBlock]["method"][0] = method;
}

std::string	VirtualServerConfig::getMethodGetLoc(int locationBlock) const
{
	return (_location[locationBlock].at("method")[0]);
}

void	VirtualServerConfig::setMethodPostLoc(std::string method, int locationBlock)
{
	_location[locationBlock]["method"][1] = method;
}

std::string	VirtualServerConfig::getMethodPostLoc(int locationBlock) const
{
	return (_location[locationBlock].at("method")[1]);
}

void	VirtualServerConfig::setMethodDeleteLoc(std::string method, int locationBlock)
{
	_location[locationBlock]["method"][2] = method;
}

std::string	VirtualServerConfig::getMethodDeleteLoc(int locationBlock) const
{
	return (_location[locationBlock].at("method")[2]);
}