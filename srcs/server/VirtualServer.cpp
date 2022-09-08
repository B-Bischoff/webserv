#include "VirtualServer.hpp"

VirtualServer::VirtualServer()
{
	// Default constructor should not be called
}

VirtualServer::VirtualServer(const VirtualServerConfig& config)
	: _config(config), _serverSocket(-1)
{
	init(); // Add verification (return, try, ...)
	std::cout << "Virtual server '" << _config.getVectorField("server_name")[0] << "' is now listening on: " << _config.getStringField("ip");
	std::cout << ":" << _config.getPort() << " with fd:" << _serverSocket << std::endl;
}

// ---------------------------------------------------
// CHANGE "exit(EXIT_FAILURE)" with exceptions throwns
// ---------------------------------------------------
void VirtualServer::init()
{
	_addrlen = sizeof(_address); // IPV4

	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = inet_addr(_config.getStringField("ip").c_str()); // Ip
	_address.sin_port = htons(_config.getPort()); // Port
	memset(_address.sin_zero, '\0', sizeof _address.sin_zero);

	// Creating socket file descriptor
	if ((_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}

	// Reuse socket even if it's "already in use"
	int yes = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
	{
		perror("In setsockopt");
		exit(EXIT_FAILURE);
	}

	if (bind(_serverSocket, (struct sockaddr *)&_address, sizeof(_address)) < 0)
		perror("In bind");
	if (listen(_serverSocket, 10) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}
}

const std::string& VirtualServer::getName() const
{
	return (_config.getVectorField("server_name"))[0];
}

const std::string& VirtualServer::getIp() const
{
	return _config.getStringField("ip");
}

unsigned int VirtualServer::getPort() const
{
	return _config.getPort();
}

const int& VirtualServer::getServerSocket() const
{
	return _serverSocket;
}

VirtualServerConfig	&VirtualServer::getVirtualServerConfig()
{
	return (_config);
}
