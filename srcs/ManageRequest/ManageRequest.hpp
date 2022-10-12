#ifndef MANAGE_REQUEST_HPP
# define MANAGE_REQUEST_HPP

#include "Server.hpp"
#include "LocationBlock.hpp"
#include "Method.hpp"
#include "CgiHandler.hpp"
#include "Get.hpp"
#include "Post.hpp"
#include "Delete.hpp"
#include "ErrorStatus.hpp"

struct Client;
class ManageRequest
{
	private:
		Client					&_client;

	public:
		ManageRequest(Client &client);
		~ManageRequest();
		
		Method			identify();
};

#endif
