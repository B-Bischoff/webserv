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
		std::string			_newPath;
		bool				_autoindex;
		bool				_redirect;
		
		void		initRequest(std::string &method, RequestHeader &request);
		bool		isValidMethod(std::string &method);
		void		getRootPath(RequestHeader &request);
		void		isDirectoryPath();
	public:
		ManageRequest(VirtualServerConfig &vServCongif, LocationBlock &locationBlock, RequestHeader &request);
		~ManageRequest();
		
		Method				identify(RequestHeader &request);
		bool			getAutoindex(void) const;
		bool			getRedirect(void) const;
		const	std::string	&getNewPath() const;
};

#endif