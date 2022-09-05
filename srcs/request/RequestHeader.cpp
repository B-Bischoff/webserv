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

	parseMethodePathAndVersion(tmp);
	tmp << request; // Go to the next request line
	parseHost(tmp);
}

void RequestHeader::parseMethodePathAndVersion(std::stringstream& tmp)
{
	//std::cout << tmp.str() << std::endl;
	tmp >> _method;
	tmp >> _path;
	// if (_path == "/")
	// 	_path = "/index.html";
	tmp >> _version;
}
// Instead of creating a function for each request (host, connection, ...)
// we could call a function depending on the request
void RequestHeader::parseHost(std::stringstream& tmp)
{
	std::string request;

	tmp >> request; 
	if (request == "Host:")
		tmp >> _host;
	else
		std::cout << "Unexpected request: " << request << std::endl;
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

std::string RequestHeader::getHost() const
{
	return (_host);
}

