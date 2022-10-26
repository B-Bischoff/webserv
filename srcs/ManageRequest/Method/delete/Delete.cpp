#include "Delete.hpp"

Delete::Delete()
{
	
}

Delete::~Delete()
{

}

Method	Delete::exec(RequestConfig &config, const std::string &body)
{
	(void)body;
	std::ifstream	ifs;

	ifs.open(config.getRootPath().c_str(), std::ios::in);
	if (ifs.is_open() == false)
		throw (STATUS_404);
	if (remove(config.getRootPath().c_str()) == -1)
		throw(STATUS_403);
	setResponseValue("", STATUS_204, _requestConfig.getRootPath());
	return (*this);
}
