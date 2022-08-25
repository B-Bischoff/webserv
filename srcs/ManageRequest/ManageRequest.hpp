#ifndef MANAGE_REQUEST_HPP
# define MANAGE_REQUEST_HPP

#include "Method.hpp"
#include "Get.hpp"
#include "Post.hpp"
#include "Delete.hpp"
#include "RequestHeader.hpp"
class ManageRequest
{
	private:
		Get		_get;
		// Post	_post;
		// Delete	_delete;

	public:
		ManageRequest();
		~ManageRequest();
		
		Method	identify(RequestHeader request);
};

#endif