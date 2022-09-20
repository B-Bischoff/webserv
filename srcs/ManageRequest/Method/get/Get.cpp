#include "Get.hpp"

Get::~Get()
{

}

Get::Get(RequestConfig &config)
{
	_requestConfig = config;
}

void	Get::readFile(RequestHeader &request)
{
	(void)request;
	std::ifstream	ifs;

	if (_requestConfig.getValidMethod() == false)
		throw(STATUS_405);
	else if (_requestConfig.getRedirect() == true)
		return (redirect(_requestConfig.getRedirectPath()));
	else if (_requestConfig.getAutoindex() == true)
		return (autoindex(_requestConfig.getRootPath()));
	ifs.open(_requestConfig.getRootPath(), std::ios::in);
	std::string		file ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		
	if (ifs.is_open() == false)
		throw (STATUS_404);
	if (file.empty() == true)
		throw (STATUS_204);
	_body = file;
	_size = _body.size();
	_status = STATUS_200;
}

void	Get::autoindex(const std::string &rootPath)
{
	Autoindex	autoindex(rootPath);

	_body = autoindex.buildAutoindex();
	_size = _body.size();
	_status = STATUS_200;
}

void	Get::redirect(const std::string &redirectUrl)
{
	_body = "<html>\n<head>\n<title>Redirection en cours</title\n<meta";
	_body += "http-equiv=\"refresh\" content=\"5; URL=";
	_body += redirectUrl;
   	_body += "\">\n</head>\n<body>\nRedirection vers ";
	_body += redirectUrl +  " dans 5 secondes.\n</body>\n</html>";
	_size = _body.size();
	_status = STATUS_301;
}
