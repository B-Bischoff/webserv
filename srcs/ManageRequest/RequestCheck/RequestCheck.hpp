#ifndef REQUEST_CHECK_HPP
# define REQUEST_CHECK_HPP

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
#include "RequestHeader.hpp"

class RequestCheck
{
	private:
	
	protected:
		int	_returnCode;

	public:
		RequestCheck() {}
		RequestCheck(RequestHeader request) {(void)request;}
		virtual ~RequestCheck() {}

};

#endif