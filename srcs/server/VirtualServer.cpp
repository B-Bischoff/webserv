#include "VirtualServer.hpp"

VirtualServer::VirtualServer()
	: _name(""), _port(0)
{
	// Default constructor should not be called
}

VirtualServer::VirtualServer(const std::string& name, const char* ip, const unsigned int& port)
	: _name(name),_ip(ip), _port(port)
{
	init(); // Add verification (return, try, ...)
	std::cout << "Virtual server '" << _name << "' is now listening on: " << _ip;
	std::cout << ":" << _port << " with fd:" << _serverSocket << std::endl;
}

void VirtualServer::init()
{
    _addrlen = sizeof(_address); // IPV4

    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = inet_addr(_ip.c_str());
    _address.sin_port = htons(_port);
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
    {
        perror("In bind");
		// exit(EXIT_FAILURE);
    }
    if (listen(_serverSocket, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
}

const std::string& VirtualServer::getName() const
{
	return _name;
}

const std::string& VirtualServer::getIp() const
{
	return _ip;
}

const unsigned int& VirtualServer::getPort() const
{
	return _port;
}

const int& VirtualServer::getServerSocket() const
{
	return _serverSocket;
}
