#ifndef MANAGE_REQUEST_HPP
# define MANAGE_REQUEST_HPP

#include "Method.hpp"
#include "Get.hpp"
#include "Post.hpp"
#include "Delete.hpp"
#include "RequestHeader.hpp"
#include "ErrorStatus.hpp"
#include "webserv.hpp"
// #include <exception>
#include <stdexcept>



class ManageRequest
{
	private:
		
	public:
		ManageRequest();
		~ManageRequest();
		
		Method	identify(RequestHeader request);
};

#endif