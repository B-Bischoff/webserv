#include "RequestConfig.hpp"

RequestConfig::RequestConfig() : _rootPath(""), _validMethod(true), _isAutoindex(false), _isCgi(false), _isRedirect(false), _redirectPath("")
{

}

RequestConfig::RequestConfig(LocationBlock &locBlock, VirtualServerConfig &confBlock, RequestHeader &request) :
	 _rootPath(""), _validMethod(true), _isAutoindex(false), _isCgi(false), _isRedirect(false), _redirectPath("")
{
	std::string	method(request.getField("Method"));
	if (isValidMethod(method, locBlock, confBlock) == false)
		_validMethod = false;
	if ((_redirectPath = locBlock.getStringField("return")) != "")
		_isRedirect = true;
	initRootPath(request, locBlock, confBlock);
	isDirectoryPath(locBlock);
	isCgi(method, locBlock, request);
}

RequestConfig::~RequestConfig()
{
}

RequestConfig &RequestConfig::getRequestConfig()
{
	return (*this);
}

bool	RequestConfig::isValidMethod(std::string &method, LocationBlock &locBlock, VirtualServerConfig &confBlock)
{
	int 	returnMethod;
	bool	serverPriority = false;

	if (!locBlock.getBoolValue(GET) && !locBlock.getBoolValue(POST)
		&& !locBlock.getBoolValue(DELETE))
		serverPriority = true;
	method == "GET" ? returnMethod = GET :
	method == "POST" ? returnMethod = POST :
	method == "DELETE" ? returnMethod = DELETE :
 	returnMethod = -1;
	if (returnMethod == -1)
		return (false);
	return (serverPriority ? confBlock.getBoolValue(returnMethod) : locBlock.getBoolValue(returnMethod));
}


void	RequestConfig::initRootPath(RequestHeader &request, LocationBlock &locBlock, VirtualServerConfig &confBlock)
{
	if (locBlock.getStringField("root") != "")
		_rootPath = locBlock.getStringField("root");
	else
		_rootPath = confBlock.getStringField("root");
	_rootPath += request.getField("Path");
}


void	RequestConfig::isDirectoryPath(LocationBlock &locBlock)
{
	if (_rootPath[_rootPath.size() - 1] != '/')
		return;
	if (locBlock.getStringField("index") != "")
		_rootPath += locBlock.getStringField("index");
	else if (locBlock.getBoolValue(AUTOINDEX) == true)
		_isAutoindex = true;
}

void	RequestConfig::isCgi(std::string &method, LocationBlock &locBlock, RequestHeader &request)
{
	if (method == "GET")
	{
		if (locBlock.getStringField("cgi_pass") != ""
			|| request.getField("Path").find('?') != std::string::npos)
				_isCgi = true;
	}
	else if (method == "POST")
		_isCgi = true;
	else
		_isCgi = false;
}

bool	RequestConfig::getAutoindex() const
{
	return (_isAutoindex);
}

bool	RequestConfig::getRedirect() const
{
	return (_isRedirect);
}

const std::string	&RequestConfig::getRootPath() const
{
	return (_rootPath); 
}

bool	RequestConfig::getCgi() const
{
	return (_isCgi);
}

const std::string	&RequestConfig::getRedirectPath() const
{
	return (_redirectPath);
}

bool	RequestConfig::getValidMethod() const
{
	return (_validMethod);
}

RequestConfig	&RequestConfig::operator=(const RequestConfig &ref)
{
	_rootPath = ref._rootPath;
	_validMethod = ref._validMethod;
	_isAutoindex = ref._isAutoindex;
	_isCgi = ref._isCgi;
	_isRedirect = ref._isRedirect;
	_redirectPath = ref._redirectPath;
	return (*this);
}