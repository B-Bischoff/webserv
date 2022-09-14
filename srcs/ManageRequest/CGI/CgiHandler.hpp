#ifndef CGI_HANDLER_HPP
# define CGI_HANDLER_HPP

#include "webserv.hpp"

class CgiHandler
{
	private:
		std::map<std::string, std::string>	_env;
		char								**_charEnv;
		char								**_args;
		std::string							_body;

		void	initCharEnv();

	public:
		CgiHandler(RequestHeader &request, VirtualServerConfig &vServ, LocationBlock &loc, std::string &method, std::string &path);
		~CgiHandler();

		std::string	execCgi();
};

#endif