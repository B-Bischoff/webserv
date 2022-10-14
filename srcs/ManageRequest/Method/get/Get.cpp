#include "Get.hpp"

Get::~Get()
{

}

Get::Get()
{

}

Method	Get::exec(RequestConfig &config, const std::string &cgiResult)
{
	std::ifstream	ifs;
	errno = 0;

	ifs.open(config.getRootPath().c_str(), std::ios::in);
	if (errno == 13)
		throw(STATUS_403);
	if (ifs.is_open() == false)
		throw (STATUS_404);
	if (config.getCgi() == false)
	{
		std::string		file ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		if (file.empty() == true)
			throw (STATUS_204);
		_body = file;
	}
	else
	{
		if (cgiResult.empty() == true)
			throw (STATUS_204);
		_header = cgiResult.substr(0, cgiResult.find("\r\n\r\n"));
		_body = cgiResult.substr(cgiResult.find("\r\n\r\n") + 4);
	}
	_size = _body.size();
	_status = STATUS_200;
	_path = config.getRootPath();
	return (*this);
}
