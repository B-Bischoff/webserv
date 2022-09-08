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

void Server::serverLoop()
{
	RequestHeader	req;
	Get				get;
	Method			dst;
	
    while(1)
    {
		std::cout << std::endl << "+++++++ Waiting for new connection ++++++++" << std::endl << std::endl;

		_readFds = _master;
		if (select(_fdmax + 1, &_readFds, NULL, NULL, NULL) == -1)
		{
			perror("Select");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i <= _fdmax; i++)
		{
			if (FD_ISSET(i, &_readFds)) // Socket ready to communicate
			{
				if (isAVirtualServer(i)) // Incoming connection from new client
					acceptConnection(i);
				else // Client wants to communicate
					listenClient(i);
			}
		}
	}
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
	int receiveReturn = receiveRequestHeader(clientFd, buf);

	if (receiveReturn <= 0) // Client disconnected or recv error
	{
		if (receiveReturn == 0)
			std::cout << "Socket: " << clientFd << " disconnected" << std::endl;
		else
			perror("recv");
		std::cout << "Socket: " << clientFd << " quit" << std::endl;
		close(clientFd);
		FD_CLR(clientFd, &_master);
	}
	else
	{
		processClientRequest(clientFd, buf);
	}
}

int Server::receiveRequestHeader(const int& clientFd, std::string& buffer)
{
	int nbytes;
	char temp;

	do
	{
		nbytes = recv(clientFd, &temp, sizeof(temp), 0);
		buffer += temp;
	} while (nbytes > 0 && buffer.find("\r\n\r\n") == std::string::npos);

	//std::cout << buffer << std::endl;
	if (buffer.find("\r\n\r\n") != std::string::npos)
		return 1; // Header successfully received
	else
		return nbytes; // Receive error or client disconnected
}

void Server::processClientRequest(const int& clientFd, std::string& buffer)
{
		RequestHeader	request;
		LocationBlock	tmp;
		int				i;

		//std::cout << buffer << std::endl;
		request.readRequest(buffer);

		// Testing virtual server identification
		VirtualServerSelector selector(_servers, request);
		i = selector.selectServerFromRequest();

		// Select location block from server and request header
		LocationSelector	select;
		tmp = select.selectLocationBlock(request.getField("Path"), this->getVirtualServer(i).getVirtualServerConfig().loc);
		// Read body from request (recv)

		ManageRequest manager(getVirtualServer(i).getVirtualServerConfig(), tmp, request);
		Method dst = manager.identify(request);
		header.build_response(dst);
		if (send(clientFd, header.response_header.c_str(), header.response_header.size(), 0) == -1)
			perror("send");
}

bool Server::isAVirtualServer(const int& fd) const
{
	return _servers.find(fd) != _servers.end();
}

VirtualServer	&Server::getVirtualServer(int i)
{
	return (_servers.at(i));
}