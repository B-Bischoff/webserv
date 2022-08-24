#ifndef DELETE_HPP
# define DELETE_HPP

#include "RequestCheck.hpp"

class Delete : public RequestCheck
{
	private:
	public:
		Delete(RequestHeader request);
		~Delete();

		virtual void	test();
};

#endif