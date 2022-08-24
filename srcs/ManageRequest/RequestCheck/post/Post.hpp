#ifndef POST_HPP
# define POST_HPP

#include "RequestCheck.hpp"

class Post : public RequestCheck
{
	private:
	public:
		Post(RequestHeader request);
		~Post();

		virtual void	test();
};

#endif