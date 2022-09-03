#include "VirtualServerConfig.hpp"

VirtualServerConfig::VirtualServerConfig() : _client_max_body_size(1000000)
{
	_block["server_name"].push_back("default");
	_block["access_log"].push_back("");
	_block["error_log"].push_back("");
	_block["listen"].push_back("0.0.0.0");
	_block["listen"].push_back("8080");
}

VirtualServerConfig::~VirtualServerConfig()
{
	
}

void	VirtualServerConfig::setIp(std::string ip)
{
	_block["listen"][0] = ip;
}

const std::string&	VirtualServerConfig::getIp(void) const
{
	return (_block.at("listen")[0]);
}

void	VirtualServerConfig::setPort(std::string port)
{
	_block["listen"][1] = port;
}

unsigned int	VirtualServerConfig::getPort(void) const
{
	return std::stoi(_block.at("listen")[1], NULL, 10);
}

void	VirtualServerConfig::setServerName(std::vector<std::string> serverName)
{
	_block["server_name"] = serverName;
}

const std::vector<std::string>&	VirtualServerConfig::getServerName(void) const
{
	return (_block.at("server_name"));
}

void	VirtualServerConfig::setAccessLog(std::vector<std::string> accessLog)
{
	_block["access_log"] = accessLog;
}

std::vector<std::string>	VirtualServerConfig::getAccessLog(void) const
{
	return (_block.at("access_log"));
}

void	VirtualServerConfig::setErrorLog(std::vector<std::string> errorLog)
{
	_block["error_log"] = errorLog;
}

std::vector<std::string>	VirtualServerConfig::getErrorLog(void) const
{
	return (_block.at("error_log"));
}

void	VirtualServerConfig::setMaxBodySize(unsigned int size)
{
	_client_max_body_size = size;
}

unsigned int	VirtualServerConfig::getMaxBodySize(void) const
{
	return (_client_max_body_size);
}
