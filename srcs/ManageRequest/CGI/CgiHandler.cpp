#include "CgiHandler.hpp"

CgiHandler::CgiHandler(std::map<std::string, std::string> &request, VirtualServerConfig &vServ, LocationBlock &loc, std::string &method)
{
	std::map<std::string, std::string>::iterator	it;
	(void)request;
	(void)vServ;
	(void)loc;
	_env["REDIRECT_STATUS"] = "200";
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_SOFTWARE"] = "Webserv/1.1";
	_env["SERVER_NAME"] = request.at("Host");
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["SERVER_PORT"] = std::to_string(vServ.getPort());
	_env["REQUEST_METHOD"] = method;
	if (loc.getRoot() != "")
		_env["PATH_TRANSLATED"] = loc.getRoot() + request.at("PATH");
	else
		_env["PATH_TRANSLATED"] = vServ.getRoot() + request.at("PATH");
	_env["PATH_INFO"] = request.at("PATH");
	_env["SCRIPT_NAME"] = "./cgi-bin/php-cgi";
	if (method == "GET" && (it = request.find("QUERY_STRING")) != request.end())
		_env["QUERY_STRING"] = request.at("QUERY_STRING");
	else
		_env["QUERY_STRING"] = "";
	if (method == "POST")
	{
		_env["CONTENT_LENGTH"] = request.at("Content-Length");
		_env["CONTENT_TYPE"] = request.at("Content-type");
	}
	// else
	// 	_env["CONTENT_LENGTH"] = "";
	// 	_env["CONTENT_TYPE"] = "";
	_env["HTTP_ACCEPT"] = request.at("Accept");
	_env["HTTP_ACCEPT_LANGUAGE"] = request.at("Accept-Language");
	_env["HTTP_USER_AGENT"] = request.at("User-Agent");
	if ((it = request.find("Referer")) != request.end())
		_env["HTTP_REFERER"] = request.at("Referer");
	if ((it = request.find("Body")) != request.end())
		_body = request.at("Body");
	else
		_body = "";
}

void	CgiHandler::initCharEnv()
{
	std::map<std::string, std::string>::iterator	it;
	int												i = 0;
	_charEnv = new char*[_env.size() + 1];
	if (!_charEnv)
		throw(STATUS_500);
	_args = new char*[3];
	if (!_args)
		throw(STATUS_500);

	for (it = _env.begin(); it != _env.end(); it++)
	{
		_charEnv[i] = strdup((it->first + "=" + it->second).c_str());
		i++;
	}
	_charEnv[i] = NULL;
}

std::string	CgiHandler::execCgi()
{
	pid_t		pid;
	int			saveStdin;
	int			saveStdout;
	std::string	response;

	saveStdin = dup(STDIN_FILENO);
	saveStdout = dup(STDOUT_FILENO);

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	int		ret = 1;

	write(fdIn, _body.c_str(), _body.size());
	lseek(fdIn, 0, SEEK_SET);

	pid = fork();
	if (pid == -1)
		throw(STATUS_500);
	else if (!pid)
	{
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(_args[0], _args, _charEnv);
		throw(STATUS_500);
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

	for (size_t i = 0; _charEnv[i]; i++)
		delete[] _charEnv[i];
	delete[] _charEnv;

	if (!pid)
		exit(0);

	return (response);
}


CgiHandler::~CgiHandler()
{

}