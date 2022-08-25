#include "RequestHeader.hpp"

RequestHeader::RequestHeader()
{
}

RequestHeader::~RequestHeader() 
{

}

void	RequestHeader::readRequest(std::string& request)
{
	std::stringstream	tmp;

	tmp << request;
	std::cout << tmp.str() << std::endl;
	tmp >> _method;
	tmp >> _path;
	if (_path == "/")
		_path = "/index.html";
	tmp >> _version;
	_path = ROOT_PATH + _path;
}

std::string	RequestHeader::getPath() const
{
	return (_path);
}

std::string	RequestHeader::getMethod() const
{
	return (_method);
}

std::string	RequestHeader::getVersion() const
{
	return (_version);
}

