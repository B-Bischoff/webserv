#include "Get.hpp"

Get::Get()
{

}

Get::~Get()
{

}

Get::Get(ManageRequest &manageRequest, const std::string &redirectPath, RequestHeader &request)
{
	_path = manageRequest.getNewPath();
	_redirect = manageRequest.getRedirect();
	_isAutoindex = manageRequest.getAutoindex();
	_redirectPath = redirectPath;
	readFile(request);
}

void	Get::readFile(RequestHeader &request)
{
	(void)request;
	std::ifstream	ifs;
	if (_redirect == true)
		return (buildRedirect());
	if (_isAutoindex == true)
		return (buildAutoindex());
	ifs.open(_path.c_str(), std::ios::in);
	std::string		file ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	
	if (ifs.is_open() == false)
		throw (STATUS_404);
	if (file.empty() == true)
		throw (STATUS_204);
	_body = file;
	_size = file.size();
	_status = STATUS_200;
}

void	Get::buildAutoindex()
{
	Autoindex	autoindex(_path);

	_isAutoindex = true;
	_body = autoindex.buildAutoindex();
	_size = _body.size();
	_status = STATUS_200;
}

void	Get::buildRedirect()
{
	_body = "<html>\n<head>\n<title>Redirection en cours</title\n<meta";
	_body += "http-equiv=\"refresh\" content=\"5; URL=";
	_body += _redirectPath;
   	_body += "\">\n</head>\n<body>\nRedirection vers ";
	_body += _redirectPath +  " dans 5 secondes.\n</body>\n</html>";
	_size = _body.size();
	_status = STATUS_301;
}