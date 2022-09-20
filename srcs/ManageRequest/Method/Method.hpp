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

#include "RequestConfig.hpp"
#include "RequestHeader.hpp"

#define	STATUS_200 "200 OK"
#define STATUS_204 "204 No Content"
#define	STATUS_301 "301 Moved Permanently"
#define STATUS_404 "404 Not Found"
#define STATUS_405 "405 Method Not Allowed"
#define STATUS_500 "500 Internal Server Error"

class Method
{
	private:
	
	protected:
		int				_size;
		std::string		_body;
		std::string 	_path;
		std::string 	_status;
		RequestConfig	_requestConfig;

	public:
		Method();
		Method(RequestHeader request);
		virtual ~Method();

		int			getSize() const;
		std::string	getBody() const;
		std::string	getPath() const;
		std::string	getStatus() const;
		RequestConfig	&getConf();

};

#endif