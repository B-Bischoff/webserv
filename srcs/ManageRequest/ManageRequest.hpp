#ifndef MANAGE_REQUEST_HPP
# define MANAGE_REQUEST_HPP

#include "VirtualServerConfig.hpp"
#include "LocationBlock.hpp"
#include "Method.hpp"
#include "CgiHandler.hpp"
#include "Get.hpp"
#include "ErrorStatus.hpp"

class ManageRequest
{
	private:
		VirtualServerConfig	&_vServConfig; // Virtual server (reference)
		LocationBlock		&_locationBlock; // Virtual server location block previously choosen (reference)
		RequestHeader		&_request;// Request header + body

	public:
		ManageRequest(VirtualServerConfig &vServCongif, LocationBlock &locationBlock, RequestHeader &request);
		~ManageRequest();
		
		Method			identify(RequestHeader &request);
};

#endif