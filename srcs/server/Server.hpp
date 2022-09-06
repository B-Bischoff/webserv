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
#include <vector>
#include <iterator>
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
#include "VirtualServerSelector.hpp"
#include "VirtualServerConfig.hpp"
#include "ErrorStatus.hpp"
#include "ResponseHeader.hpp"
#include "Parsing.hpp"

class Server {
private:
	std::map<int, VirtualServer> _servers;

	int _newSocket;
	ResponseHeader	header; // Probably gonna make that variable temporary

	fd_set _master, _readFds;
	int _fdmax;

	void createVirtualServer(const VirtualServerConfig& config);

	void serverLoop();

	void acceptConnection(const int& serverSocket);
	void listenClient(const int& clientFd);
	void processClientRequest(const int& clientFd, std::string& buffer);

	int receiveRequestHeader(const int& cliendFd, std::string& buffer);
	bool isAVirtualServer(const int& fd) const;
	void addFd(const int& fd, fd_set& set);

public:
	Server(const std::vector<VirtualServerConfig>& configList);

};

#endif
