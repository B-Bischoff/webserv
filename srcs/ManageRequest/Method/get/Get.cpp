#include "Get.hpp"

Get::Get()
{
}

Get::~Get()
{

}

Get	&Get::readFile(RequestHeader request)
{
	_path = "./pages";
	if (request.getPath() == "/")
		_path += "/index.html";
	else
		_path += request.getPath();
	std::cout << _path<< std::endl;
	std::ifstream	ifs(_path, std::ios::in);
	std::string		file ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	
	if (ifs.is_open() == false)
		throw (STATUS_404);
	if (file.empty() == true)
		throw (STATUS_204);
	_body = file;
	_size = file.size();
	_status = STATUS_200;
	return (*this);
}