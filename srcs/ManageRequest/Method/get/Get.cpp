#include "Get.hpp"


Get::Get()
{

}


Get::~Get()
{

}

Method	Get::exec(RequestConfig &config, std::string &cgiResult)
{
	std::ifstream	ifs;
	errno = 0;
	_requestConfig = config;

	ifs.open(_requestConfig.getRootPath().c_str(), std::ios::in);
	if (errno == 13)
		throw(STATUS_403);
	if (ifs.is_open() == false)
		throw (STATUS_404);
	if (_requestConfig.getCgi() == false)
	{
		std::string		file ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		if (file.empty() == true)
			throw (STATUS_204);
		_body = file;
	}
	else
	{
		_cgiHeader = cgiResult.substr(0, cgiResult.find("\r\n\r\n"));
		cgiResult.erase(0, cgiResult.find_first_of("\r\n\r\n"));
		_body = cgiResult;
		if (_body.empty() == true)
			throw (STATUS_204);
	}
	_size = _body.size();
	_status = STATUS_200;
	_path = _requestConfig.getRootPath();
	return (*this);
}
