#include "VirtualServConfig.hpp"

VirtualServConfig::VirtualServConfig() : _client_max_body_size(1000000), _autoIndex(false)
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

VirtualServConfig::~VirtualServConfig()
{
	
}

void	VirtualServConfig::setListenIp(std::string ip)
{
	std::cout << ip << std::endl;
	_vServ["listen"][0] = ip;
}

std::string	VirtualServConfig::getListenIp(void) const
{
	return (_vServ.at("listen")[0]);
}

void	VirtualServConfig::setListenPort(std::string port)
{
	_vServ["listen"][0] = port;
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
	_vServ["root"][0] = path;
}

std::string	VirtualServConfig::getRoot(void) const
{
	return (_vServ.at("root")[0]);
}

void	VirtualServConfig::setIndex(std::string path)
{
	_vServ["index"][0] = path;
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
	_vServ["error_log"] = errorLog;
}

std::vector<std::string>	VirtualServConfig::getErrorLog(void) const
{
	return (_vServ.at("error_log"));
}

void	VirtualServConfig::setMethodGet(std::string method)
{
	_vServ["method"][0] = method;
}

std::string	VirtualServConfig::getMethodGet(void) const
{
	return (_vServ.at("method")[0]);
}

void	VirtualServConfig::setMethodPost(std::string method)
{
	_vServ["method"][1] = method;
}

std::string	VirtualServConfig::getMethodPost(void) const
{
	return (_vServ.at("method")[1]);
}

void	VirtualServConfig::setMethodDelete(std::string method)
{
	_vServ["method"][2] = method;
}

std::string	VirtualServConfig::getMethodDelete(void) const
{
	return (_vServ.at("method")[2]);
}

void	VirtualServConfig::setAutoIndex(bool value)
{
	_autoIndex = value;
}

bool	VirtualServConfig::getAutoIndex(void) const
{
	return (_autoIndex);
}

void	VirtualServConfig::setReturn(std::vector<std::string> value)
{
	_vServ["return"] = value;
}

std::vector<std::string>	VirtualServConfig::getReturn(void) const
{
	return (_vServ.at("return"));
}

void	VirtualServConfig::setMaxBodySize(unsigned int size)
{
	_client_max_body_size = size;
}

unsigned int	VirtualServConfig::getMaxBodySize(void) const
{
	return (_client_max_body_size);
}

void	VirtualServConfig::setAutoIndexLoc(bool value, int locationBlock)
{
	_location[locationBlock]["autoindex"].push_back(std::to_string(value));
}

bool	VirtualServConfig::getAutoIndexLoc(int locationBlock) const
{
	return ((_location[locationBlock].at("autoindex").at(0) == "off" ? true : false));
}

void	VirtualServConfig::setReturnLoc(std::vector<std::string> value, int locationBlock)
{
	_location[locationBlock]["return"] = value;
}

std::vector<std::string>	VirtualServConfig::getReturnLoc(int locationBlock) const
{
	return (_location[locationBlock].at("return"));
}


void	VirtualServConfig::setRootLoc(std::string path, int locationBlock)
{
	_location[locationBlock]["root"][0] = path;
}

std::string	VirtualServConfig::getRootLoc(int locationBlock) const
{
	return (_location[locationBlock].at("root")[0]);
}

void	VirtualServConfig::setIndexLoc(std::string path, int locationBlock)
{
	_location[locationBlock]["index"][0] = path;
}

std::string	VirtualServConfig::getIndexLoc(int locationBlock) const
{
	return (_location[locationBlock].at("index")[0]);
}

void	VirtualServConfig::setMethodGetLoc(std::string method, int locationBlock)
{
	_location[locationBlock]["method"][0] = method;
}

std::string	VirtualServConfig::getMethodGetLoc(int locationBlock) const
{
	return (_location[locationBlock].at("method")[0]);
}

void	VirtualServConfig::setMethodPostLoc(std::string method, int locationBlock)
{
	_location[locationBlock]["method"][1] = method;
}

std::string	VirtualServConfig::getMethodPostLoc(int locationBlock) const
{
	return (_location[locationBlock].at("method")[1]);
}

void	VirtualServConfig::setMethodDeleteLoc(std::string method, int locationBlock)
{
	_location[locationBlock]["method"][2] = method;
}

std::string	VirtualServConfig::getMethodDeleteLoc(int locationBlock) const
{
	return (_location[locationBlock].at("method")[2]);
}