#ifndef CGI_HANDLER_HPP
# define CGI_HANDLER_HPP

#include "webserv.hpp"
#include <sys/wait.h>
#include <sys/types.h>
#include "Server.hpp"

struct Client;

class CgiHandler
{
	private:
		static	void	initCharEnv(std::map<std::string, std::string> &_env, char **_charEnv, char **_args);
		static	void setGlobalCgiVariable(const Client &client, std::map<std::string, std::string>	&_env);

	public:

		static std::string	execCgi(const Client &client);
};

#endif
