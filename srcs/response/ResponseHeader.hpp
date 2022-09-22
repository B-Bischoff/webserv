#ifndef RESPONSE_HEADER_HPP
# define RESPONSE_HEADER_HPP

#include <iostream>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <string>
#include <signal.h>
#include "Method.hpp"

class ResponseHeader
{
	private:
		std::string	protocol;
		std::string	content_type;
		std::string	content_length;
		std::string	get_extension_file(std::string path);
		
	public:
		std::string	response_header;
		int		content_size;
		bool closeAfterSend;
		ResponseHeader();
		~ResponseHeader();
		void	build_response(Method &method);
};

#endif