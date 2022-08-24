#include "RequestHeader.hpp"

RequestHeader::RequestHeader()
{
}

RequestHeader::~RequestHeader() 
{

}

void	RequestHeader::readRequest(int socket)
{
	char				buf[MAX_BODY_SIZE] = {0};
	std::stringstream	tmp;

	read(socket, &buf , MAX_BODY_SIZE);
	tmp << buf;
	std::cout << tmp.str() << std::endl;
	tmp >> _method;
	tmp >> _path;
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

