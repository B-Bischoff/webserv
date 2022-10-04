#include "ErrorStatus.hpp"

ErrorStatus::ErrorStatus()
{

}

ErrorStatus::~ErrorStatus()
{
	
}

ErrorStatus	&ErrorStatus::buildError(std::string error, VirtualServerConfig &vServ)
{
	std::string		path;
	if ((findStatus(vServ.getErrorStatus(), atoi(error.c_str())) == false)
		|| ((path = buildErrorPath(vServ.getStringField("error_path"), error)) == ""))
	{
		_body = "<div id=\"main\">\n\t<div class=\"fof\">\n\t\t<h1></h1>\n\t</div>\n</div>";
		_body.insert(41, error);
	}
	else
		_body = getErrorPage(path);
	_size = _body.size();
	_status = error;
	return (*this);
}

bool	ErrorStatus::findStatus(const std::vector<int> &status, int statusCode)
{
	for (std::vector<int>::const_iterator it = status.begin(); it != status.end(); it++)
	{
		if (*it == statusCode)
			return (true);
	}
	return (false);
}

std::string	ErrorStatus::buildErrorPath(std::string path, const std::string &error)
{
	std::fstream	ifs;

	path += error.substr(0, error.find_first_of(' '));
	path += ".html";
	ifs.open(path.c_str(), std::ios::in);
	if (ifs.is_open() == false)
		return ("");
	ifs.close();
	return (path);
}


std::string	ErrorStatus::getErrorPage(std::string path)
{
	std::ifstream	ifs;
	std::string		file;

	ifs.open(path.c_str(), std::ios::in);
	file = std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	return (file);
}
