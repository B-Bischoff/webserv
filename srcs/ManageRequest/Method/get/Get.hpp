#ifndef GET_HPP
# define GET_HPP

#include "Method.hpp"

class Get : public Method
{
	private:

	public:
		Get();
		~Get();

		Get	&readFile(RequestHeader request);
};

#endif