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
#include "ManageRequest.hpp"
#include "VirtualServer.hpp"
#include "VirtualServerSelector.hpp"
#include "VirtualServerConfig.hpp"
#include "ResponseHeader.hpp"
#include "Parsing.hpp"
#include "LocationSelector.hpp"
#include "SocketCommunicator.hpp"

struct Client {
	RequestHeader request;
	ResponseHeader response;
	bool needToReceiveBody;
	int virtualServerId;
	std::string body; // To delete
};

class Server {
private:
	std::map<int, VirtualServer> _servers;

	int _newSocket;

	fd_set _master;
	int _fdmax;

	std::map<int, Client> _clients;

	void createVirtualServer(const VirtualServerConfig& config);

	void serverLoop();

	void acceptConnection(const int& serverSocket);
	int listenClient(const int& clientFd);
	int listenHeader(const int& clientFd);
	void listenBody(const int& clientFd);
	const Method processClientRequest(const int& clientFd);
	void createClientResponse(const int& clientFd, Method& method);

	bool needToReceiveBody(const RequestHeader& request) const;
	bool isAVirtualServer(const int& fd) const;
	void addFd(const int& fd, fd_set& set);
	void removeFd(const int& fd, fd_set& set);
	fd_set createWriteFdSet() const;

public:
	Server(const std::vector<VirtualServerConfig>& configList);

};

#endif
