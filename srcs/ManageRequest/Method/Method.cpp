#include "Method.hpp"

Method::Method(RequestHeader request) : _closeAfterSend(false)
{
	(void)request;
}

Method::Method() : _closeAfterSend(false)
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

Method	&Method::redirect(const std::string &redirectUrl)
{
	_body = "<html>\n<head>\n<title>Redirection en cours</title\n<meta";
	_body += "http-equiv=\"refresh\" content=\"5; URL=";
	_body += redirectUrl;
   	_body += "\">\n</head>\n<body>\nRedirection vers ";
	_body += redirectUrl +  " dans 5 secondes.\n</body>\n</html>";
	_size = _body.size();
	_status = STATUS_301;
	return (*this);
}

bool	Method::getCloseAfterSend() const
{
	return _closeAfterSend;
}

void	Method::setCloseAfterSend(const bool& value)
{
	_closeAfterSend = value;
}
