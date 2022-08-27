#include "Server.hpp"

Server::Server() : _fdmax(0)
{
	serverInit();
	serverLoop();
}

void Server::serverInit()
{
	serverSocketInit();
	FD_ZERO(&_master);
	addFd(_serverSocket, _master);
}

void Server::serverSocketInit()
{
    _addrlen = sizeof(_address); // IPV4

    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(PORT);
    memset(_address.sin_zero, '\0', sizeof _address.sin_zero);

    // Creating socket file descriptor
    if ((_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
	 
	// Reuse socket even if it's "already in use"
	int yes = 1;
	setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    
    if (bind(_serverSocket, (struct sockaddr *)&_address, sizeof(_address)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(_serverSocket, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
}

void Server::serverLoop()
{
	ManageRequest	manager;
	RequestHeader	req;
	Get				get;
	Method			dst;
	
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
				if (i == _serverSocket) // Server detects incoming connection
				{
					acceptConnection();
				}
				else // Client wants to communicate
				{
					processClientRequest(i);
				}
			}
		}
    }
}

void Server::acceptConnection()
{
	_newSocket = accept(_serverSocket, (struct sockaddr*)&_address, (socklen_t*)&_addrlen);
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
		header.build_response(dst);
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
