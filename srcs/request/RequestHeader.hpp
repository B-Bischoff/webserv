#ifndef REQUEST_HEADER_HPP
# define REQUEST_HEADER_HPP

#include <iostream>
#include <istream>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <map>
#define MAX_BODY_SIZE 30000
#define ROOT_PATH "./pages"


class RequestHeader
{
	private:
		std::map<std::string, std::string> _fields;

		std::string	_method;
		std::string	_path;
		std::string	_version;
		std::string _host;

		void parseMethodPathAndVersion(std::string& line);
		void parseField(std::string& line);

	public:
		RequestHeader();
		~RequestHeader();

		const std::string&	getField(const std::string& field) const;

		void			readRequest(std::string& request);
};

#endif
