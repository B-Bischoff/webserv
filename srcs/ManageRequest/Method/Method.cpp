#include "Method.hpp"

Method::Method(const RequestConfig &requestConfig) : _closeAfterSend(false), _requestConfig(requestConfig)
{

}

Method::Method()
{

}

Method::~Method()
{

}

int	Method::getSize() const
{
	return (this->_size);
}

std::string	Method::getBody() const
{
	return (this->_body);
}

std::string	Method::getPath() const
{
	return (this->_path);
}

std::string	Method::getStatus() const
{
	return (this->_status);
}

RequestConfig	&Method::getConf()
{
	return (this->_requestConfig);
}

Method	&Method::autoindex(const std::string &rootPath, const std::string &fullPath)
{
	Autoindex	autoindex(rootPath, fullPath);

	_body = autoindex.buildAutoindex();
	_size = _body.size();
	_status = STATUS_200;
	return (*this);
}

Method	&Method::redirect(const RequestConfig &requestConfig)
{
	_requestConfig = requestConfig;
	_body = "<html>\n<head>\n<title>Redirection en cours</title\n<meta";
	_body += "http-equiv=\"refresh\" content=\"5; URL=";
	_body += requestConfig.getRedirectPath();
   	_body += "\">\n</head>\n<body>\nRedirection vers ";
	_body += requestConfig.getRedirectPath() +  " dans 5 secondes.\n</body>\n</html>";
	_size = _body.size();
	_status = STATUS_301;
	return (*this);
}

void	Method::setResponseValue(std::string body, std::string status, std::string path)
{
	_body = body;
	_size = body.size();
	_status = status;
	_path = path;
}

bool	Method::getCloseAfterSend() const
{
	return _closeAfterSend;
}

void	Method::setCloseAfterSend(const bool& value)
{
	_closeAfterSend = value;
}

std::string	Method::getHeader() const
{
	return (this->_cgiHeader);
}