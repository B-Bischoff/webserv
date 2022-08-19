#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <string>

#define PORT 8080

class Server {
private:
	int _serverFd, _newSocket, _valRead, _addrlen;
	struct sockaddr_in _address;

	void serverInit();
	void serverLoop();

public:
	Server();

};

#endif
