#include "RequestHeader.hpp"

RequestHeader::RequestHeader(std::string request_content)
{
	std::stringstream	tmp; 
	
	tmp << request_content;

	tmp >> _method;	
	tmp >> _path;	
	tmp >> _version;	
}

RequestHeader::~RequestHeader() 
{

}

std::string	RequestHeader::getPath() const
{
	return (_path);
}

