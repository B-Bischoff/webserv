#include "ManageRequest.hpp"

Method ManageRequest::identify(RequestHeader request)
{
	if (request.getMethod() == "GET")
	{
		return (_get.readFile(request));
	}
	else if (request.getMethod() == "POST")
	{

	}
	else if (request.getMethod() == "DELETE")
	{

	}
	else
	{

	}
	return (_get);
}

ManageRequest::ManageRequest()
{

}

ManageRequest::~ManageRequest()
{

}