#include "CgiHandler.hpp"

CgiHandler::CgiHandler(RequestHeader &request, VirtualServerConfig &vServ, LocationBlock &loc, const std::string &method, const std::string &path, std::string &body)
{
	(void)vServ;
	_body = body;
	_env["SERVER_SOFTWARE"] = "Webserv/1.1";
	_env["SERVER_NAME"] = request.getField("Host").substr(0, request.getField("Host").find_first_of(':'));
	_env["GATEWAY_INTERFACE"] = "PHP/8.1.9";

	_env["REDIRECT_STATUS"] = "200";
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_PORT"] = request.getField("Host").substr(request.getField("Host").find_first_of(':') + 1);
	_env["REQUEST_METHOD"] = method;
	_env["PATH_INFO"] = request.getField("Path").substr(0, request.getField("Path").find_first_of('?'));
	_env["PATH_TRANSLATED"] = path.substr(0, path.find_first_of('?'));
	_env["SCRIPT_NAME"] = loc.getStringField("cgi_pass");
	if (method == "GET")
		_env["QUERY_STRING"] = request.getField("Path").substr(request.getField("Path").find_first_of('?') + 1);
	if (method == "POST")
	{
		if (request.getField("Content-Length") == "")
			throw (STATUS_411);
		_env["CONTENT_LENGTH"] = request.getField("Content-Length");
		_env["CONTENT_TYPE"] = request.getField("Content-Type");
	}
	_env["HTTP_METHOD"] = method;
	if (request.getField("Accept").empty() == false)
		_env["HTTP_ACCEPT"] = request.getField("Accept");
	if (request.getField("Accept-Language").empty() == false)
		_env["HTTP_ACCEPT_LANGUAGE"] = request.getField("Accept-Language");
	if (request.getField("User-Agent").empty() == false)
		_env["HTTP_USER_AGENT"] = request.getField("User-Agent");
	if (request.getField("Referer").empty() == false)
		_env["HTTP_REFERER"] = request.getField("Referer");
}

void	CgiHandler::initCharEnv()
{
	std::map<std::string, std::string>::iterator	it = _env.begin();
	int												i = 0;
	_charEnv = new char*[_env.size() + 1];
	if (!_charEnv)
		throw("malloc charenv");
	_args = new char*[3];
	if (!_args)
		throw("malloc args");
	while (it != _env.end())
	{
		_charEnv[i] = strdup((it->first + "=" + it->second).c_str());
		std::cout << _charEnv[i] << std::endl;
		i++;
		it++;
	}
	std::cout << "end of initCharEnv" << std::endl;
	_charEnv[i] = NULL;
	_args[0] = (char *)"./cgi-bin/php-cgi";
	_args[1] = (char *)_env["Path"].c_str();
	_args[2] = NULL;
}

std::string	CgiHandler::execCgi()
{
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
	write(fdIn, _body.c_str(), _body.size());
	lseek(fdIn, 0, SEEK_SET);
	initCharEnv();
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

	std::cout << "Cgi response:" << std::endl;
	std::cout << "================================" << std::endl;
	std::cout << response << std::endl;
	std::cout << "================================" << std::endl;
	return (response);
}


CgiHandler::~CgiHandler()
{

}