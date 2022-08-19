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
    if ((_serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons( PORT );
    
    memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
    
    if (bind(_serverFd, (struct sockaddr *)&_address, sizeof(_address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(_serverFd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
}

void Server::serverLoop()
{
	std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 5000\n\n";

	std::ifstream ifs("pages/index.html", std::ios::in);
	std::string webPageContent ((std::istreambuf_iterator<char>(ifs)), 
			(std::istreambuf_iterator<char>()));

	hello += webPageContent;


    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((_newSocket = accept(_serverFd, (struct sockaddr *)&_address, (socklen_t*)&_addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
		_valRead = read(_newSocket, &buffer, 30000);
		std::string buf = buffer;
		std::cout << buf << std::endl;

        write(_newSocket , hello.c_str() , hello.length());
        printf("------------------Hello message sent-------------------");
        close(_newSocket);
    }
}
