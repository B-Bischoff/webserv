#include "VirtualServerConfig.hpp"

VirtualServerConfig::VirtualServerConfig() : _client_max_body_size(1000000)
{

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

void	VirtualServerConfig::setErrorStatus(std::vector<int> values)
{
	_errorStatus = values;
}
