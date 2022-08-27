#include "Server.hpp"

Server::Server()
{
	serverInit();
	serverLoop();
}

void Server::serverInit()
{
	FD_ZERO(&_master); // Clear master set

	// Virtual servers will soon be loaded from config file
	createVirtualServer("MainServer", 8080);
	createVirtualServer("SecondaryServer", 9090);
}

void Server::createVirtualServer(const std::string &name, const unsigned int& port)
{
	VirtualServer virtualServer(name, port); // Check virtual server creation

	_servers.push_back(virtualServer);
	addFd(virtualServer.getServerSocket(), _master);
}

void Server::serverLoop()
{
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");

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
				{
					acceptConnection(i);
				}
				else // Client wants to communicate
				{
					// Redirect client request to the correct virtual server
					// The virtual server depends of the request header (port, servName, ...)
					processClientRequest(i);
				}
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
		std::cout << "New incoming connection (fd: " << _newSocket << ")" << std::endl;
		fcntl(_newSocket, F_SETFL, O_NONBLOCK); // Set fd to non-blockant (prg will not get stuck on recv)
		addFd(_newSocket, _master);
	}
}

void Server::processClientRequest(const int& fd)
{

	std::cout << "client " << fd << " wants to communicate" << std::endl;
	char buffer[30000];
	int nbytes = recv(fd, buffer, sizeof(buffer), 0);
	if (nbytes <= 0)
	{
		if (nbytes == 0)
			std::cout << "Socket: " << fd << " disconnected" << std::endl;
		else
			perror("recv");
		std::cout << "Socket: " << fd << " quit" << std::endl;
		close(fd);
		FD_CLR(fd, &_master);
	}
	else
	{
		RequestHeader	request;

		std::string buf = buffer;
		std::cout << buf << std::endl;
		request.readRequest(buf);
		ManageRequest manager;
		Method dst = manager.identify(request);
		header.build_response(dst.getPath(), dst.getBody(), dst.getSize(), dst.getStatus());
		//std::cout << header.response_header << std::endl;

		if (send(fd, header.response_header.c_str(), header.response_header.size(), 0) == -1)
			perror("send");

		//close (fd);
		//FD_CLR(fd, &_master);
	}
}

void Server::addFd(const int &fd, fd_set& set)
{
	FD_SET(fd, &set);
	if (fd > _fdmax)
		_fdmax = fd;
}

bool Server::isAVirtualServer(const int& fd) const
{
	// Might use iterator
	for (unsigned int i = 0; i < _servers.size(); i++)
		if (fd == _servers[i].getServerSocket())
			return true;
	return false;
}
