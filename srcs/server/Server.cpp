#include "Server.hpp"

Server::Server()
{
	serverInit();
	serverLoop();
}

void Server::serverInit()
{
	serverSocketInit();

	FD_SET(_serverSocket, &_master);
	_fdmax = _serverSocket;

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
				std::cout << "socket " << i << " is set" << " | fd max: " << _fdmax << std::endl;
				if (i == _serverSocket) // Server detects incoming connection
				{
					_newSocket = accept(_serverSocket, (struct sockaddr*)&_address, (socklen_t*)&_addrlen);
					if (_newSocket == -1)
					{
						perror("accept");
					}
					else
					{
						std::cout << "New incoming connection (fd: " << _newSocket << ")" << std::endl;
						FD_SET(_newSocket, &_master);
						std::cout << _newSocket << " added to master" << std::endl;
						if (_newSocket > _fdmax)
							_fdmax = _newSocket;
					}
				}
				else // Client wants to communicate
				{
					std::cout << "client" << i << " wants to communicate" << std::endl;
					char buffer[30000];
					int nbytes = recv(i, buffer, sizeof(buffer), 0);
					if (nbytes <= 0)
					{
						if (nbytes == 0)
							std::cout << "Socket: " << i << " disconnected" << std::endl;
						else
							perror("recv");
						std::cout << "Socket: " << i << " quit" << std::endl;
						close(i);
						FD_CLR(i, &_master);
					}
					else
					{
						//std::string buf = buffer;
						RequestHeader	request;

						std::string buf = buffer;
						request.readRequest(buf);
						ManageRequest manager;
						Method dst = manager.identify(request);
						header.build_response(dst.getPath(), dst.getBody(), dst.getSize(), dst.getStatus());
						std::cout << header.response_header << std::endl;

						std::cout << "----> " << header.response_header.size() << " | " << dst.getSize() << std::endl;
						
						if (send(i, header.response_header.c_str(), header.response_header.size(), 0) == -1)
							perror("send");


						// close(i);
						// FD_CLR(i, &_master);
						std::cout << "client " << i << "closed after data send" << std::endl;
					}
				}
			}
		}
    }
}
