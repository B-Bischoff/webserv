#include "RequestConfig.hpp"

RequestConfig::RequestConfig() : _validMethod(true), _isAutoindex(false), _isCgi(false), _isRedirect(false), _rootPath(""), _redirectPath(""), _upload(""), _method("")
{
}

RequestConfig::RequestConfig(LocationBlock &locBlock, VirtualServerConfig &confBlock, RequestHeader &request) :
	_validMethod(true), _isAutoindex(false), _isCgi(false), _isRedirect(false), _rootPath(""), _redirectPath(""), _upload(""), _method("")
{
	_method = request.getField("Method");
	if (isValidMethod(_method, locBlock, confBlock) == false)
		_validMethod = false;
	if ((_redirectPath = locBlock.getStringField("return")) != "")
		_isRedirect = true;
	if (locBlock.getStringField("upload") != "")
		_upload = locBlock.getStringField("upload");
	initRootPath(request, locBlock, confBlock);
	isDirectoryPath(locBlock);
	isCgi(locBlock);
}

RequestConfig::~RequestConfig()
{
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
	_rootPath += request.getField("Path").substr(0, request.getField("Path").find_first_of('?'));
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

void	RequestConfig::isCgi(LocationBlock &locBlock)
{
	if (locBlock.getStringField("cgi_pass") != "")
		_isCgi = true;
}

bool	RequestConfig::getAutoindex() const
{
	return (_isAutoindex);
}

bool	RequestConfig::getRedirect() const
{
	return (_isRedirect);
}

bool	RequestConfig::getCgi() const
{
	return (_isCgi);
}

bool	RequestConfig::getValidMethod() const
{
	return (_validMethod);
}

const std::string	&RequestConfig::getRedirectPath() const
{
	return (_redirectPath);
}

const std::string	&RequestConfig::getRootPath() const
{
	return (_rootPath); 
}

const std::string	&RequestConfig::getMethod() const
{
	return (_method);
}

const std::string	&RequestConfig::getUpload() const
{
	return (_upload);
}
