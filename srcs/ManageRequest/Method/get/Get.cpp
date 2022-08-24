#include "Get.hpp"

Get::Get()
{
}

Get::~Get()
{

}

Get	&Get::readFile(RequestHeader request)
{
	std::ifstream	ifs(request.getPath(), std::ios::in);
	std::string		file ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	
	_path = request.getPath();
	_body = file;
	_size = file.size();
	_status = "200 OK\n";
	return (*this);
}