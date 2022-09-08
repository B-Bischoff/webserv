#include "VirtualServerConfig.hpp"

VirtualServerConfig::VirtualServerConfig() : _client_max_body_size(1000000)
{
	_block["server_name"].push_back("default");
	_block["access_log"].push_back("");
	_block["error_log"].push_back("");
	_block["ip"].push_back("0.0.0.0");
}

VirtualServerConfig::~VirtualServerConfig()
{
	
}

void	VirtualServerConfig::setPort(std::string port)
{
	_port = atoi(port.c_str());
}

unsigned int	VirtualServerConfig::getPort(void) const
{
	return _port;
}

void	VirtualServerConfig::setMaxBodySize(unsigned int size)
{
	_client_max_body_size = size;
}

unsigned int	VirtualServerConfig::getMaxBodySize(void) const
{
	return (_client_max_body_size);
}
