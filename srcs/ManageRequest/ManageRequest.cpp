#include "ManageRequest.hpp"

ManageRequest::ManageRequest(VirtualServerConfig &vServCongif, LocationBlock &locationBlock, RequestHeader &request) :
	_vServConfig(vServCongif), _locationBlock(locationBlock), _request(request), _autoindex(false)
{
	(void)_vServConfig;
	(void)_locationBlock;
	(void)_request;
}

ManageRequest::~ManageRequest()
{

}
/*
	==== GENERATE RESPONSE ALGO ====

	use default root / path
	check method (from location block or virtual server)
	cgi ?
	search file from location

	return response header (string)
*/

Method ManageRequest::identify(RequestHeader &request)
{
	Method empty;
	std::string	method(request.getField("Method"));
	try
	{
		initRequest(method, request);
		CgiHandler	cgi(_request, _vServConfig, _locationBlock, method, _newPath);
		std::string	responseCgi = cgi.execCgi();
		if (method == "GET")
		{
			Get	get(*this, _locationBlock.getStringField("return"), request);
			return (get);
		}
		else if (method == "POST")
		{

		}
		else if (method == "DELETE")
		{

		}
		else
		{

		}
	}
	catch(const char *e)
	{
		ErrorStatus	error;
		return (error.buildError(e));
	}
	catch(...)
	{
		ErrorStatus	error;
		return (error.buildError(STATUS_500));
	}
	return (empty);
}

void	ManageRequest::initRequest(std::string &method, RequestHeader &request)
{
	if (isValidMethod(method) == false)
		throw(STATUS_405);
	if (_locationBlock.getStringField("return") != "")
		_redirect = true;
	getRootPath(request);
	isDirectoryPath();
}


bool	ManageRequest::isValidMethod(std::string &method)
{
	int 	returnMethod;
	bool	serverPriority = false;

	if (!_locationBlock.getBoolValue(GET) && !_locationBlock.getBoolValue(POST)
		&& !_locationBlock.getBoolValue(DELETE))
		serverPriority = true;
	method == "GET" ? returnMethod = GET :
	method == "POST" ? returnMethod = POST :
	method == "DELETE" ? returnMethod = DELETE :
 	returnMethod = -1;
	if (returnMethod == -1)
		return (false);
	return (serverPriority ? _vServConfig.getBoolValue(returnMethod) : _locationBlock.getBoolValue(returnMethod));
}

void	ManageRequest::getRootPath(RequestHeader &request)
{
	if (_locationBlock.getStringField("root") != "")
		_newPath = _locationBlock.getStringField("root");
	else
		_newPath = _vServConfig.getStringField("root");
	_newPath += request.getField("Path");
}

void	ManageRequest::isDirectoryPath()
{
	if (_newPath[_newPath.size() - 1] != '/')
		return;
	if (_locationBlock.getStringField("index") != "")
		_newPath += _locationBlock.getStringField("index");
	else if (_locationBlock.getBoolValue(AUTOINDEX) == true)
		_autoindex = true;
}

bool	ManageRequest::getAutoindex() const
{
	return (_autoindex);
}

bool	ManageRequest::getRedirect() const
{
	return (_redirect);
}

const std::string	&ManageRequest::getNewPath() const
{
	return (_newPath); 
}