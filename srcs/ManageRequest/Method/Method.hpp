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
#include "Autoindex.hpp"
#include "StatusCode.hpp"

class Method
{
	private:
		bool 			_closeAfterSend;
	
	protected:
		int				_size;
		std::string		_body;
		std::string 	_path;
		std::string 	_status;
		std::string		_cgiHeader;
		RequestConfig	_requestConfig;

	public:
		Method();
		Method(const RequestConfig &requestConfig);
		virtual ~Method();

		Method			exec(RequestConfig &config, const std::string &body);
		Method			&autoindex(const std::string &rootPath, const std::string &fullPath);
		Method			&redirect(const RequestConfig &requestConfig);
		RequestConfig	&getConf();
		int				getSize() const;
		std::string		getBody() const;
		std::string		getHeader() const;
		std::string		getPath() const;
		std::string		getStatus() const;
		bool			getCloseAfterSend() const;
		void			setCloseAfterSend(const bool& value);

};

#endif
