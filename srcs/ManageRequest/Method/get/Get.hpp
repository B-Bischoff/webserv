#ifndef GET_HPP
# define GET_HPP

#include "Server.hpp"
#include "Autoindex.hpp"
#include "ManageRequest.hpp"

class ManageRequest;
class Get : public Method
{
	private:
		void	buildAutoindex();
		void	buildRedirect();
		void	readFile(RequestHeader &request);
		
	public:
		Get(ManageRequest &manageRequest, const std::string &redirectPath, RequestHeader &request);
		~Get();
		Get();

};

#endif