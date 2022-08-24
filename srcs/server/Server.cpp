#include "Server.hpp"

Server::Server()
{
	serverInit();
	serverLoop();
}

void Server::serverInit()
{
	(void)_valRead;
    _addrlen = sizeof(_address);

    // Creating socket file descriptor
    if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
	int yes = 1;
	setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    _address.sin_family = AF_INET; 
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons( PORT );
    
    memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
    
    if (bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(_server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
}

void Server::serverLoop()
{
	RequestHeader	req;
	
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((_new_socket = accept(_server_fd, (struct sockaddr *)&_address, (socklen_t*)&_addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
		req.readRequest(_new_socket);
		// if (req.getMethod() == "GET")
		Get	get(req);
		// else if (req.getMethod() == "POST")
			// Post post(req);
		// else if (req.getMethod() == "DELETE")
			// Delete del(req);
		// else
			// std::cerr << "error" << std::endl;
		header.build_response(get._path, get._body, get._size, get._status);
        write(_new_socket , header.response_header.c_str() , get._size);
        printf("------------------Hello message sent-------------------");
        close(_new_socket);
    }
}
