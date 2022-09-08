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
		_env["PATH_TRANSLATED"] = loc.getStringField("root") + request.getField("Path");
	else
		_env["PATH_TRANSLATED"] = vServ.getStringField("root") + request.getField("Path");
	_env["PATH_INFO"] = request.getField("Path");
	_env["SCRIPT_NAME"] = "./cgi-bin/php-cgi";
	// if (method == "GET" && (it = request.find("QUERY_STRING")) != request.end())
		// _env["QUERY_STRING"] = request.getField("QUERY_STRING");
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

	for (it = _env.begin(); it != _env.end(); it++)
	{
		_charEnv[i] = strdup((it->first + "=" + it->second).c_str());
		i++;
	}
	_charEnv[i] = NULL;
	_args[0] = (char *)"./cgi-bin/php-cgi";
	_args[1] = (char *)_env["Path"].c_str();
	_args[2] = NULL;
}

std::string	CgiHandler::execCgi()
{
	pid_t		pid;
	int			saveStdin;
	int			saveStdout;
	std::string	response;

	saveStdin = dup(STDIN_FILENO);
	saveStdout = dup(STDOUT_FILENO);
	initCharEnv();

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	int		ret = 1;

	write(fdIn, _body.c_str(), _body.size());
	lseek(fdIn, 0, SEEK_SET);

	pid = fork();
	if (pid == -1)
		throw("pid");
	else if (!pid)
	{
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		std::cerr << "Args value: " << _args[0] << " " << _args[1] << std::endl;
		execve(_args[0], _args, _charEnv);
		throw("execve");
	}
	else
	{
		char	buffer[1000000] = {0};

		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, 1000000);
			ret = read(fdOut, buffer, 1000000 - 1);
			response += buffer;
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

	// for (size_t i = 0; _charEnv[i]; i++)
	// 	delete[] _charEnv[i];
	// delete[] _charEnv;

	if (!pid)
		exit(0);

	return (response);
}


CgiHandler::~CgiHandler()
{

}