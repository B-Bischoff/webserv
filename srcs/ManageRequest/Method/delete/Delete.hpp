#ifndef DELETE_HPP
# define DELETE_HPP

#include "Method.hpp"

class Delete : public Method
{
	private:
	
	public:
		Delete();
		~Delete();

		Method	exec(RequestConfig &config, const std::string &body);
};

#endif