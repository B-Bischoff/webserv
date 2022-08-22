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
#include <signal.h>
#include "webserv.hpp"
#define PORT 8080


class Server {
private:
	int _valRead, _addrlen, _server_fd, _new_socket;
	struct sockaddr_in _address;
	ResponseHeader	header;

	void serverInit();
	void serverLoop();

public:
	Server();

};

#endif
