#include "Method.hpp"

Method::Method(RequestHeader request)
{
	_isAutoindex = false;
	_redirect = false;
	_redirectPath = "";
	(void)request;
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

bool	Method::getAutoindex() const
{
	return (this->_isAutoindex);
}

bool	Method::getRedirect() const
{
	return (this->_redirect);
}

std::string	Method::getRedirectPath() const
{
	return (this->_redirectPath);
}