#ifndef GET_HPP
# define GET_HPP

#include "RequestCheck.hpp"

class Get : public RequestCheck
{
	private:
	public:
		Get(RequestHeader request);
		~Get();
		std::string	_body;
		std::string _path;
		std::string _status;
		int			_size;

		virtual void	test();
};

#endif