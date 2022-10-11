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
	std::string	body;

	if (requestConfig.getValidMethod() == false)
		throw(STATUS_405);
	else if (requestConfig.getRedirect() == true)
		return (index.redirect(requestConfig.getRedirectPath()));
	else if (requestConfig.getAutoindex() == true)
		return (index.autoindex(requestConfig.getRootPath()));
	else if (requestConfig.getCgi() == true)
	{
		CgiHandler cgi(request, _vServConfig, _locationBlock, requestConfig.getMethod(), requestConfig.getRootPath(), _body);
		body = cgi.execCgi();
	}
	if (requestConfig.getMethod() == "GET")
	{
		Get	get;
		return (get.exec(requestConfig, body));
	}
	else if (requestConfig.getMethod() == "POST")
	{
		Post post;
		return (post.exec(requestConfig, _body, atoi(request.getField("Content-Length").c_str())));
	}
	else
	{
		Delete	del;
		return (del.exec(requestConfig, body));
	}
	return (index);
}



