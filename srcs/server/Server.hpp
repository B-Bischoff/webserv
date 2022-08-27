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
#include "poll.h"
#include "fcntl.h"
#include "webserv.hpp"
#include "RequestHeader.hpp"
#include "Method.hpp"
#include "Get.hpp"
#include "Post.hpp"
#include "Delete.hpp"
#include "ManageRequest.hpp"
#include "ErrorStatus.hpp"
#include "ResponseHeader.hpp"

#define PORT 8080

/*
	The server will from now use the select function to handle connections
	It requires to keep track the highest fd
	It is also necessary to create three 

*/

class Server {
private:
	int _addrlen;
	int _serverSocket, _newSocket;
	struct sockaddr_in _address;
	ResponseHeader	header;

	fd_set _master, _readFds;
	int _fdmax = 0;

	void serverInit();
	void serverSocketInit();

	void serverLoop();

	void addFd(const int& fd, fd_set& set);
	void acceptConnection();
	void processClientRequest(const int& fd);

public:
	Server();

};

#endif
