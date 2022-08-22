#include "Server.hpp"

Server::Server()
{
	serverInit();
	serverLoop();
}

void Server::serverInit()
{
    _addrlen = sizeof(_address);

    // Creating socket file descriptor
    if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

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

    while(1)
    {
		// Just change path here, for now accept html and png and that's it!
		//Put the build_response here will make sure that if we make change inside the file
		//change will be applied on the client page
		header.build_response("pages/gif.gif");
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((_new_socket = accept(_server_fd, (struct sockaddr *)&_address, (socklen_t*)&_addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
		_valRead = read(_new_socket, &buffer, 30000);
		std::string buf = buffer;
		std::cout << buf << std::endl;

        write(_new_socket , header.response_header.c_str() , header.content_size);
        printf("------------------Hello message sent-------------------");
        close(_new_socket);
    }
}
