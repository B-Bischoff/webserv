#include "ManageRequest.hpp"

/*
	==== GENERATE RESPONSE ALGO ====

	use default root / path
	check method (from location block or virtual server)
	cgi ?
	search file from location

	return response header (string)
*/

Method ManageRequest::identify(RequestHeader request)
{
	Method empty;

	try
	{
		if (request.getMethod() == "GET")
		{
			Get	get;
			get.readFile(request);
			return (get);
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

ManageRequest::ManageRequest()
{

}

ManageRequest::~ManageRequest()
{

}