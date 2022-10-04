#ifndef GET_HPP
# define GET_HPP

#include "Method.hpp"

class RequestConfig;

class Get : public virtual Method
{
	private:
		
	public:
		Get();
		~Get();

		Method	exec(RequestConfig &config, const std::string &body);
};

#endif
