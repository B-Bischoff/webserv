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
		CgiHandler(std::map<std::string, std::string> &request, VirtualServerConfig &vServ, LocationBlock &loc, std::string &method);
		~CgiHandler();

		std::string	execCgi();
};

#endif