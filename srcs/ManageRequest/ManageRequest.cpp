#include "ManageRequest.hpp"

ManageRequest::ManageRequest(VirtualServerConfig &vServCongif, LocationBlock &locationBlock, RequestHeader &request) :
	_vServConfig(vServCongif), _locationBlock(locationBlock), _request(request)
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
	// std::string	method("GET");
	// CgiHandler	cgi(_request, _vServConfig, _locationBlock, method);
	// std::string	responseCgi = cgi.execCgi();

	// std::cout << "Cgi response: " << std::endl;
	// std::cout << responseCgi << std::endl;
	std::cout << "\nIs valid method return : " << isValidMethod(request.getField("Method")) << std::endl;
	try
	{
		if (request.getField("Method") == "GET")
		{
			Get	get;
			get.readFile(request);
			return (get);
		}
		else if (request.getField("Method") == "POST")
		{

		}
		else if (request.getField("Method") == "DELETE")
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

bool	ManageRequest::isValidMethod(std::string method)
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
