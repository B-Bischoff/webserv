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
#include "VirtualServer.hpp"
#include <vector>
#include <iterator>

class Server {
private:
	// Might use Map instead of vector to identify virtual servers from their socket 
	std::vector<VirtualServer> _servers;

	int _newSocket;
	ResponseHeader	header;

	fd_set _master, _readFds;
	int _fdmax = 0;

	void serverInit();
	void createVirtualServer(const std::string& name, const unsigned int& port);

	void serverLoop();

	void addFd(const int& fd, fd_set& set);
	void acceptConnection(const int& serverSocket);
	void processClientRequest(const int& fd);

	bool isAVirtualServer(const int& fd) const;

public:
	Server();

};

#endif
