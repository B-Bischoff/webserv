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
		//std::cout << "Exception catched" << std::endl;
		// std::cout << e.what();
		return (error.buildError(e));
	}
	catch(...)
	{
		std::cout << "Unknown Exception catched" << std::endl;
	}
	return (empty);
}

ManageRequest::ManageRequest()
{

}

ManageRequest::~ManageRequest()
{

}