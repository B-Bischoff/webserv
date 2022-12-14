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
	RequestHeader 		request;
	ResponseHeader 		response;
	size_t 				bytesSent;
	size_t				bytesRead;
	bool 				needToReceiveBody;
	VirtualServerConfig	virtualServer;
	LocationBlock		locationBlock;
	int 				virtualServerId;
	std::string 		body;
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

	void manageClientRequest(const int& clientFd);
	int listenClient(const int& clientFd);
	int listenHeader(const int& clientFd);
	int listenBody(const int& clientFd);
	const Method processClientRequest(const int& clientFd);
	void createClientResponseFromMethod(const int& clientFd, Method& method);
	Method manageError(const int& clientFd, const char* e);

	void respondToClient(const int& clientFd);

	bool needToReceiveBody(const RequestHeader& request) const;
	bool isAVirtualServer(const int& fd) const;
	void addFd(const int& fd, fd_set& set);
	void removeFd(const int& fd, fd_set& set);
	fd_set createWriteFdSet() const;

public:
	Server(const std::vector<VirtualServerConfig>& configList);

};

#endif
