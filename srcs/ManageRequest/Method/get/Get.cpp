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
	std::string		file ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	if (errno == 13)
		throw(STATUS_403);
	if (ifs.is_open() == false)
		throw (STATUS_404);
	if (_requestConfig.getCgi() == false)
	{
		if (file.empty() == true)
			throw (STATUS_204);
	}
	else
	{
		_cgiHeader = cgiResult.substr(0, cgiResult.find("\r\n\r\n"));
		cgiResult.erase(0, cgiResult.find_first_of("\r\n\r\n"));
		_body = cgiResult;
		if (_body.empty() == true)
			throw (STATUS_204);
	}
	setResponseValue(_requestConfig.getCgi() == false ? file : cgiResult, STATUS_200, _requestConfig.getRootPath());
	return (*this);
}
