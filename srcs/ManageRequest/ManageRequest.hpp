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
#include "Server.hpp"
#include "LocationBlock.hpp"
#include "CgiHandler.hpp"

class ManageRequest
{
	private:
		VirtualServerConfig	&_vServConfig; // Virtual server (reference)
		LocationBlock		&_locationBlock; // Virtual server location block previously choosen (reference)
		RequestHeader		&_request;// Request header + body
		
	public:
		ManageRequest(VirtualServerConfig &vServCongif, LocationBlock &locationBlock, RequestHeader &request);
		~ManageRequest();
		
		Method	identify(RequestHeader request);
};

#endif