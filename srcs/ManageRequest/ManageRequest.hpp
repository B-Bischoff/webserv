#ifndef MANAGE_REQUEST_HPP
# define MANAGE_REQUEST_HPP

#include "Method.hpp"
#include "Get.hpp"
#include "Post.hpp"
#include "Delete.hpp"
#include "RequestHeader.hpp"
#include "ErrorStatus.hpp"
#include "webserv.hpp"
#include <exception>



class ManageRequest
{
	private:
		// Virtual server (reference)
		// Virtual server location block previously choosen (reference)
		// Request header + body
		
	public:
		ManageRequest();
		~ManageRequest();
		
		Method	identify(RequestHeader request);
};

#endif