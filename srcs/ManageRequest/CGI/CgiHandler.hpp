#ifndef CGI_HANDLER_HPP
# define CGI_HANDLER_HPP

#include "webserv.hpp"
#include <sys/wait.h>
#include <sys/types.h>

class CgiHandler
{
	private:
		std::map<std::string, std::string>	_env;
		char								**_charEnv;
		char								**_args;
		std::string							_body;

		void	initCharEnv();

	public:
		CgiHandler(RequestHeader &request, VirtualServerConfig &vServ, LocationBlock &loc, const std::string &method, const std::string &path, std::string &body);
		~CgiHandler();

		std::string	execCgi();
};

#endif
