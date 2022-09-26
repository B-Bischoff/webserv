#ifndef ERROR_STATUS_HPP
# define ERROR_STATUS_HPP

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

class ErrorStatus : public Method
{
	private:
	
	public:
		ErrorStatus();
		~ErrorStatus();

		ErrorStatus	&buildError(std::string error, VirtualServerConfig &vServ);
		bool		findStatus(const std::vector<int> &status, int status_code);
		std::string	buildErrorPath(std::string path, const std::string &error);
		std::string	getErrorPage(std::string path);
};

#endif