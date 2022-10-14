#include "CgiHandler.hpp"

void	CgiHandler::setGlobalCgiVariable(const Client &client, std::map<std::string, std::string> &_env)
{
	_env["SERVER_SOFTWARE"] = "Webserv/1.1";
	_env["SERVER_NAME"] = client.request.getField("Host").substr(0, client.request.getField("Host").find_first_of(':'));
	_env["GATEWAY_INTERFACE"] = "PHP/8.1.9";

	_env["REDIRECT_STATUS"] = "200";
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_PORT"] = client.request.getField("Host").substr(client.request.getField("Host").find_first_of(':') + 1);
	_env["REQUEST_METHOD"] = client.request.getField("Method");
	_env["PATH_INFO"] = client.request.getField("Path").substr(0, client.request.getField("Path").find_first_of('?'));
	_env["PATH_TRANSLATED"] = client.virtualServer.getStringField("root") + _env["PATH_INFO"];
	_env["SCRIPT_NAME"] = client.locationBlock.getStringField("cgi_pass");
	if (client.request.getField("Method") == "GET")
	{
		_env["QUERY_STRING"] = "";
		if (client.request.getField("Path").find('?') != std::string::npos)
			_env["QUERY_STRING"] = client.request.getField("Path").substr(client.request.getField("Path").find_first_of('?') + 1);
	}
	if (client.request.getField("Method") == "POST")
	{
		if (client.request.getField("Content-Length") == "")
			throw (STATUS_411);
		_env["CONTENT_LENGTH"] = client.request.getField("Content-Length");
		_env["CONTENT_TYPE"] = client.request.getField("Content-Type");
	}
	_env["HTTP_METHOD"] = client.request.getField("Method");
	if (client.request.getField("Accept").empty() == false)
		_env["HTTP_ACCEPT"] = client.request.getField("Accept");
	if (client.request.getField("Accept-Language").empty() == false)
		_env["HTTP_ACCEPT_LANGUAGE"] = client.request.getField("Accept-Language");
	if (client.request.getField("User-Agent").empty() == false)
		_env["HTTP_USER_AGENT"] = client.request.getField("User-Agent");
	if (client.request.getField("Referer").empty() == false)
		_env["HTTP_REFERER"] = client.request.getField("Referer");
}

void	CgiHandler::initCharEnv(std::map<std::string, std::string> &_env, char **_charEnv, char **_args)
{
	std::map<std::string, std::string>::iterator	it = _env.begin();
	int												i = 0;
	
	while (it != _env.end())
	{
		_charEnv[i] = strdup((it->first + "=" + it->second).c_str());
		std::cout << _charEnv[i] << std::endl;
		i++;
		it++;
	}
	std::cout << "end of initCharEnv" << std::endl;
	_charEnv[i] = NULL;
	_args[0] = (char *)_env["SCRIPT_NAME"].c_str();
	_args[1] = (char *)_env["PATH_TRANSLATED"].c_str();
	_args[2] = NULL;
}

std::string	CgiHandler::execCgi(const Client &client)
{
	std::map<std::string, std::string>	_env;
	char								**_charEnv = NULL;
	char								**_args = NULL;

	setGlobalCgiVariable(client, _env);
	_charEnv = new char*[_env.size() + 1];
	if (!_charEnv)
		throw("malloc charenv");
	_args = new char*[3];
	if (!_args)
		throw("malloc args");
	pid_t		pid;
	std::string	response;
	int			saveStdin;
	int			saveStdout;
	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	int		ret = 1;

	saveStdin = dup(STDIN_FILENO);
	saveStdout = dup(STDOUT_FILENO);
	write(fdIn, client.body.c_str(), client.body.size());
	lseek(fdIn, 0, SEEK_SET);
	initCharEnv(_env, _charEnv, _args);
	pid = fork();
	if (pid == -1)
		throw("pid");
	else if (!pid)
	{
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(_args[0], _args, _charEnv);
		throw("execve");
	}
	else
	{
		char	c;

		waitpid(pid, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		ret = 1;
		while (ret > 0)
		{
			ret = read(fdOut, &c, 1);
			response += c;
		}
	}
	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	fclose(fIn);
	fclose(fOut);
	close(fdIn);
	close(fdOut);
	close(saveStdin);
	close(saveStdout);
	if (!pid)
		exit(0);

	// std::cout << "Cgi response:" << std::endl;
	// std::cout << "================================" << std::endl;
	// std::cout << response << std::endl;
	// std::cout << "================================" << std::endl;
	return (response);
}