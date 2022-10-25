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
	if (config.getRootPath().find("/uploads/") == std::string::npos)
		throw (STATUS_403);
	if (remove(config.getRootPath().c_str()) == -1)
		throw(STATUS_403);
	_status = STATUS_204;
	return (*this);
}
