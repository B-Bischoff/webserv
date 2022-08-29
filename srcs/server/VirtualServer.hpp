#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class VirtualServer 
{
	private:
		VirtualServer();
	
		const std::string _name;
		const std::string _ip;
		const unsigned int _port;

		int _serverSocket = -1;
		int _addrlen;
		struct sockaddr_in _address;

		// Add file location
		// Add allowed methods
		// Add log file (?)

		void init();

	public:
		VirtualServer(const std::string& name, const char* ip, const unsigned int& port);

		const std::string& getName() const;
		const std::string& getIp() const;
		const unsigned int& getPort() const;
		const int& getServerSocket() const;
};

#endif
