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

class Method
{
	private:
	
	protected:
		int			_size;
		int			_statusValue;
		std::string	_body;
		std::string _path;
		std::string _status;

	public:
		Method();
		Method(RequestHeader request);
		virtual ~Method();

		int			getSize() const;
		std::string	getBody() const;
		std::string	getPath() const;
		std::string	getStatus() const;

		void		updateStatus(Method method, int statusValue);

};

#endif