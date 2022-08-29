#include "Server.hpp"

Server::Server() : _fdmax(0)
{
	serverInit();
	serverLoop();
}

void Server::serverInit()
{
	FD_ZERO(&_master); // Clear master set

	// Virtual servers will soon be loaded from config file
	createVirtualServer("MainServer", "127.0.0.1", 8080);
	createVirtualServer("SecondaryServer", "127.0.0.5", 9090);
}

void Server::createVirtualServer(const std::string &name, const char* ip, const unsigned int& port)
{
	VirtualServer virtualServer(name, ip, port); // Check virtual server creation

	_servers.push_back(virtualServer);
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
	ManageRequest	manager;
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
				{
					acceptConnection(i);
				}
				else // Client wants to communicate
				{
					// Redirect client request to the correct virtual server
					// The virtual server depends of the request header (port, servName, ...)
					listenClient(i);
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

void Server::listenClient(const int& clientFd)
{

	std::cout << "client " << clientFd << " wants to communicate" << std::endl;
	char buffer[30000];
	int nbytes = recv(clientFd, buffer, sizeof(buffer), 0);
	if (nbytes <= 0) // Client disconnected or recv error
	{
		if (nbytes == 0)
			std::cout << "Socket: " << clientFd << " disconnected" << std::endl;
		else
			perror("recv");
		std::cout << "Socket: " << clientFd << " quit" << std::endl;
		close(clientFd);
		FD_CLR(clientFd, &_master);
	}
	else
	{
		std::string buf = buffer; // Convert char* to string
		processClientRequest(clientFd, buf);
	}
}

void Server::processClientRequest(const int& clientFd, std::string& buffer)
{
		RequestHeader	request;

		std::cout << buffer << std::endl;
		request.readRequest(buffer);

		// Testing virtual server identification
		identifyServerFromRequest(request);

		ManageRequest manager;
		Method dst = manager.identify(request);
		header.build_response(dst);
		if (send(clientFd, header.response_header.c_str(), header.response_header.size(), 0) == -1)
			perror("send");
}

const VirtualServer& Server::identifyServerFromRequest(const RequestHeader& request) const
{
	std::string host = request.getHost();
	if (host.find(':') != std::string::npos) // host contains port
	{
		std::string ip = host.substr(0, host.find(':'));
		std::string port = host.substr(host.find(':') + 1, host.length() - 1);

		for (int i = 0; i < static_cast<int>(_servers.size()); i++)
		{
			if (ip == _servers[i].getIp() && port == std::to_string(_servers[i].getPort()))
			{
				std::cout << _servers[i].getName() << " should process request." << std::endl;
				return _servers[i];
			}
		}
	}

	// Default server should be returned

	return _servers[0];
}

bool Server::isAVirtualServer(const int& fd) const
{
	// Might use iterator
	for (unsigned int i = 0; i < _servers.size(); i++)
		if (fd == _servers[i].getServerSocket())
			return true;
	return false;
}
