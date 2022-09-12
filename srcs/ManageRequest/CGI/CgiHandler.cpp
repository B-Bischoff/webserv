#include "CgiHandler.hpp"

CgiHandler::CgiHandler(RequestHeader &request, VirtualServerConfig &vServ, LocationBlock &loc, std::string &method)
{
	std::map<std::string, std::string>::iterator	it;
	(void)request;
	(void)vServ;
	(void)loc;
	_env["REDIRECT_STATUS"] = "200";
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_SOFTWARE"] = "Webserv/1.1";
	_env["SERVER_NAME"] = request.getField("Host");
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["SERVER_PORT"] = std::to_string(vServ.getPort());
	_env["REQUEST_METHOD"] = method;
	if (loc.getStringField("root") != "")
		_env["PATH_TRANSLATED"] = loc.getStringField("root") + request.getField("Path").substr(0, request.getField("Path").find_first_of('?'));
	else
		_env["PATH_TRANSLATED"] = vServ.getStringField("root") + request.getField("Path").substr(0, request.getField("Path").find_first_of('?'));
	_env["PATH_INFO"] = request.getField("Path").substr(0, request.getField("Path").find_first_of('?'));
	_env["SCRIPT_NAME"] = loc.getStringField("cgi_pass");
	std::cout << "Path: ================= " << vServ.getStringField("cgi_pass") << std::endl;
	if (request.getField("Path").find('?') != std::string::npos)
	_env["QUERY_STRING"] = request.getField("Path").substr(request.getField("Path").find_first_of('?') + 1);
	// else
		// _env["QUERY_STRING"] = "";
	if (method == "POST")
	{
		_env["CONTENT_LENGTH"] = request.getField("Content-Length");
		_env["CONTENT_TYPE"] = request.getField("Content-type");
	}
	// else
	// 	_env["CONTENT_LENGTH"] = "";
	// 	_env["CONTENT_TYPE"] = "";
	_env["HTTP_ACCEPT"] = request.getField("Accept");
	_env["HTTP_ACCEPT_LANGUAGE"] = request.getField("Accept-Language");
	_env["HTTP_USER_AGENT"] = request.getField("User-Agent");
	// if ((it = request.find("Referer")) != request.end())
		// _env["HTTP_REFERER"] = request.getField("Referer");
	// if ((it = request.find("Body")) != request.end())
		// _body = request.getField("Body");
	// else
		// _body = "";
}

void	CgiHandler::initCharEnv()
{
	std::map<std::string, std::string>::iterator	it;
	int												i = 0;
	_charEnv = new char*[_env.size() + 1];
	if (!_charEnv)
		throw("malloc charenv");
	_args = new char*[3];
	if (!_args)
		throw("malloc args");

	std::cout << "charenv: ==============================\n" << std::endl;
	for (it = _env.begin(); it != _env.end(); it++)
	{
		_charEnv[i] = strdup((it->first + "=" + it->second).c_str());
		std::cout << _charEnv[i] << std::endl;
		i++;
	}
	std::cout << "charenv: ==============================\n" << std::endl;
	_charEnv[i] = NULL;
	_args[0] = (char *)"./cgi-bin/php-cgi";
	_args[1] = (char *)_env["Path"].c_str();
	_args[2] = NULL;
}

std::string	CgiHandler::execCgi()
{
	pid_t		pid;
	std::string	response;
	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	int		ret = 1;

	initCharEnv();
	pid = fork();
	if (pid == -1)
		throw("pid");
	else if (!pid)
	{
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(_args[0], NULL, _charEnv);
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

	fclose(fIn);
	fclose(fOut);
	close(fdIn);
	close(fdOut);
	if (!pid)
		exit(0);

	std::cout << "Cgi response: \n\n" << std::endl;
	std::cout << "================================" << std::endl;
	std::cout << response << std::endl;
	std::cout << "================================" << std::endl;
	return (response);
}


CgiHandler::~CgiHandler()
{

}