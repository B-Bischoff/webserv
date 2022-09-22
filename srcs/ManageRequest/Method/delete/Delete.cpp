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

	ifs.open(config.getRootPath(), std::ios::in);
	if (ifs.is_open() == false)
		throw (STATUS_404);
	if (config.getRootPath().find("/uploads/") == std::string::npos)
		throw (STATUS_403);
	remove(config.getRootPath().c_str());
	_status = STATUS_204;
	return (*this);
}