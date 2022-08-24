#include "Get.hpp"

Get::Get(RequestHeader request)
{
	std::ifstream	ifs(request.getPath(), std::ios::in);
	std::string		fileContent ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	
	if (!ifs.is_open())
	{
		ifs.open("./error/404/404.html", std::ios::in);
		std::string		file ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		_body = file;
		_size = file.size();
		_path = "./error/404/404.html";
		_status = "404 Not Found\n";
		return;
	}
	_path = request.getPath();
	_body = fileContent;
	_size = fileContent.size();
	_status = "200 OK\n";
}

Get::~Get()
{

}

void	Get::test()
{

}