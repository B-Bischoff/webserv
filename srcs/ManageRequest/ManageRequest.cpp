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
	std::string	method(request.getField("Method"));
	RequestConfig requestConfig(_locationBlock, _vServConfig, request);

	try
	{
		if (method == "GET")
		{
			Get	get(requestConfig.getRequestConfig());
			get.readFile(request);
			return (get);
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



