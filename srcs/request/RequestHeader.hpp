#ifndef REQUEST_HEADER_HPP
# define REQUEST_HEADER_HPP

#include <iostream>
#include <istream>
#include <sstream>


class RequestHeader
{
	private:
		std::string	_method;
		std::string	_path;
		std::string	_version;
		//Version?

		RequestHeader();

	public:
		RequestHeader(std::string request_content);
		~RequestHeader();

		std::string	getPath() const;
};

#endif
