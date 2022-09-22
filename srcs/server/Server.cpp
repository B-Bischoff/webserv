#include "Server.hpp"

Server::Server(const std::vector<VirtualServerConfig>& configList) : _fdmax(0)
{
	FD_ZERO(&_master); // Clear master set

	for (int i = 0; i < (int)configList.size(); i++)
		createVirtualServer(configList[i]);

	serverLoop();
}

void Server::createVirtualServer(const VirtualServerConfig& config)
{
	VirtualServer virtualServer(config); // Check virtual server creation

	_servers.insert(std::pair<int, VirtualServer>(virtualServer.getServerSocket(), virtualServer));
	addFd(virtualServer.getServerSocket(), _master);
}

void Server::addFd(const int &fd, fd_set& set)
{
	FD_SET(fd, &set);
	if (fd > _fdmax)
		_fdmax = fd;
}

void Server::removeFd(const int &fd, fd_set& set)
{
	FD_CLR(fd, &set);
	close(fd);
}

void Server::serverLoop()
{
    while(1)
    {
		std::cout << std::endl << "+++++++ Waiting for new connection ++++++++" << std::endl << std::endl;

		std::cout << "Response ready: " << _clientsReponse.size() << std::endl;

		fd_set readFds = _master;
		fd_set writeFds = createWriteFdSet();
		if (select(_fdmax + 1, &readFds, &writeFds, NULL, NULL) == -1)
		{
			perror("Select");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i <= _fdmax; i++)
		{
			if (FD_ISSET(i, &readFds)) // Socket ready to communicate
			{
				if (isAVirtualServer(i)) // Incoming connection from new client
					acceptConnection(i);
				else // Client wants to communicate
					listenClient(i);
			}
			if (FD_ISSET(i, &writeFds))
			{
				std::cout << "Client " << i << " ready to receive response" << std::endl;
				const ResponseHeader& response = _clientsReponse[i];
				if (send(i, response.response_header.c_str(), response.response_header.size(), 0) == -1)
					perror("send");
				_clientsReponse.erase(i);
				if (response.closeAfterSend == true)
					removeFd(i, _master);
			}
		}
	}
}

fd_set Server::createWriteFdSet() const
{
	fd_set writeFds;
	FD_ZERO(&writeFds);

	std::map<int, ResponseHeader>::const_iterator it;
	for (it = _clientsReponse.begin(); it != _clientsReponse.end(); it++)
		FD_SET(it->first, &writeFds);

	return writeFds;
}

void Server::acceptConnection(const int& serverSocket)
{
	_newSocket = accept(serverSocket, NULL, NULL);
	if (_newSocket == -1)
		perror("accept");
	else
	{
		std::cout << "New incoming connection (fd: " << _newSocket << ")";
		std::cout << " received from fd: " << serverSocket << std::endl;
		fcntl(_newSocket, F_SETFL, O_NONBLOCK); // Set fd to non-blockant (prg will not get stuck on recv)
		addFd(_newSocket, _master);
	}
}

void Server::listenClient(const int& clientFd)
{
	std::cout << "client " << clientFd << " wants to communicate" << std::endl;

	std::string buf;
	int receiveReturn = SocketCommunicator::receiveRequestHeader(clientFd, buf);
	std::cout << "Request Header: " << buf << std::endl;

	if (receiveReturn <= 0) // Client disconnected or recv error
	{
		if (receiveReturn == 0)
			std::cout << "Socket: " << clientFd << " disconnected" << std::endl;
		else
			perror("recv");
		std::cout << "Socket: " << clientFd << " quit" << std::endl;
		removeFd(clientFd, _master);
	}
	else
		processClientRequest(clientFd, buf);
}

void Server::processClientRequest(const int& clientFd, std::string& buffer)
{
		RequestHeader	request;
		LocationBlock	tmp;
		int				i;

		request.readRequest(buffer);

		VirtualServerSelector selector(_servers, request);
		i = selector.selectServerFromRequest();

		LocationSelector	select;
		tmp = select.selectLocationBlock(request.getField("Path"), _servers.at(i).getVirtualServerConfig().loc);

		std::string requestBody;
		if (SocketCommunicator::receiveRequestBody(clientFd, requestBody, request, _servers.at(i).getVirtualServerConfig().getMaxBodySize()) == -1)
		{
			std::cout << "Recv (request body) error" << std::endl;
			removeFd(clientFd, _master);
			return;
		}
		std::cout << "Request body: " << requestBody << std::endl;

		ManageRequest manager(_servers.at(i).getVirtualServerConfig(), tmp, request, requestBody);
		Method dst = manager.identify(request);
		header.build_response(dst);

		if (request.getField("Connection") == "close")
			header.closeAfterSend = true;

		_clientsReponse[clientFd] = header;
}

bool Server::isAVirtualServer(const int& fd) const
{
	return _servers.find(fd) != _servers.end();
}
