#include "ManageRequest.hpp"

ManageRequest::ManageRequest(VirtualServerConfig &vServCongif, LocationBlock &locationBlock, RequestHeader &request, std::string &body) :
	_vServConfig(vServCongif), _locationBlock(locationBlock), _request(request), _body(body)
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
	Method index;
	RequestConfig requestConfig(_locationBlock, _vServConfig, request);
	std::string	cgiResult;

	if (requestConfig.getValidMethod() == false)
		throw(STATUS_405);
	else if (requestConfig.getRedirect() == true)
		return (index.redirect(requestConfig.getRedirectPath()));
	else if (requestConfig.getAutoindex() == true)
		return (index.autoindex(_locationBlock.getStringField("root") != ""
		? _locationBlock.getStringField("root") : _vServConfig.getStringField("root"), requestConfig.getRootPath()));
	else if (requestConfig.getCgi() == true)
	{
		CgiHandler cgi(request, _vServConfig, _locationBlock, requestConfig.getMethod(), requestConfig.getRootPath(), _body);
		cgiResult = cgi.execCgi();
	}
	if (requestConfig.getMethod() == "GET")
	{
		Get	get;
		return (get.exec(requestConfig, cgiResult));
	}
	else if (requestConfig.getMethod() == "POST")
	{
		Post post;
		return (post.exec(requestConfig, _request.getBodydata()));
	}
	else
	{
		Delete	del;
		return (del.exec(requestConfig, cgiResult));
	}
	return (index);
}



