#ifndef REQUEST_HEADER_HPP
# define REQUEST_HEADER_HPP

#include <iostream>
#include <istream>
#include <sstream>
#include <unistd.h>
#include <fstream>
#define MAX_BODY_SIZE 30000
#define ROOT_PATH "./pages"


class RequestHeader
{
	private:
		std::string	_method;
		std::string	_path;
		std::string	_version;

	public:
		RequestHeader();
		~RequestHeader();

		std::string		getPath() const;
		std::string		getMethod() const;
		std::string		getVersion() const;

		void			readRequest(int socket);
};

#endif
