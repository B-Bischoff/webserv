#include "Server.hpp"

int server_fd = 0;
int	new_socket = 0;

void	my_handler(int signal)
{
	std::cout << "Caught signal " << signal << std::endl;
	shutdown(server_fd, SHUT_RDWR);
	shutdown(new_socket, SHUT_RDWR);
	close(server_fd);
	close(new_socket);
	exit(0);
}

Server::Server()
{
	signal(SIGINT, my_handler);
	serverInit();
	serverLoop();
}

void Server::serverInit()
{
    _addrlen = sizeof(_address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons( PORT );
    
    memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
    
    if (bind(server_fd, (struct sockaddr *)&_address, sizeof(_address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
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
		header.build_response("pages/video.webm");
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&_address, (socklen_t*)&_addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
		_valRead = read(new_socket, &buffer, 30000);
		std::string buf = buffer;
		std::cout << buf << std::endl;

        write(new_socket , header.response_header.c_str() , header.content_size);
        printf("------------------Hello message sent-------------------");
        close(new_socket);
    }
}
