#include "ManageRequest.hpp"

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