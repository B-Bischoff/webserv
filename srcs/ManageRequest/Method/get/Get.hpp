#ifndef GET_HPP
# define GET_HPP

#include "Method.hpp"
#include "Autoindex.hpp"

class RequestConfig;

class Get : public Method
{
	private:

		void	autoindex(const std::string &rootPath);
		void	redirect(const std::string &redirectUrl);
		
	public:
		~Get();

		Get(RequestConfig &requestConfig);
		void	readFile(RequestHeader &request);

};

#endif