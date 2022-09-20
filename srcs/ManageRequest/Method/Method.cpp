#include "Method.hpp"

Method::Method(RequestHeader request)
{
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

RequestConfig	&Method::getConf()
{
	return (this->_requestConfig);
}