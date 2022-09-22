#include "Get.hpp"

Get::~Get()
{

}

Get::Get()
{

}

Method	Get::exec(RequestConfig &config, const std::string &body)
{
	std::ifstream	ifs;

	ifs.open(config.getRootPath(), std::ios::in);
		
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
		if (body.empty() == true)
			throw (STATUS_204);
		_body = body;
	}
	_size = _body.size();
	_status = STATUS_200;
	return (*this);
}