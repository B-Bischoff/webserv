#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "VirtualServerConfig.hpp"

class VirtualServer 
{
	private:
		VirtualServer();
	
		VirtualServerConfig _config; // Removed const, got compilation error

		int _serverSocket;
		int _addrlen;
		struct sockaddr_in _address;

		void init();

	public:
		VirtualServer(const VirtualServerConfig& config);

		const std::string& getName() const;
		const std::string& getIp() const;
		unsigned int getPort() const;
		const int& getServerSocket() const;
		VirtualServerConfig	&getVirtualServerConfig();
};

#endif
