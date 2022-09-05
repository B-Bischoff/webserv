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
	
	if (ifs.is_open() == false)
	{
		//std::cout << "throw it" << std::endl;
		throw (ERROR_404);
	}
	_path = "./pages" + request.getPath();
	if (file.empty() == true)
	{
		throw (STATUS_204);
	}
	_body = file;
	_size = file.size();
	_status = STATUS_200;
	return (*this);
}