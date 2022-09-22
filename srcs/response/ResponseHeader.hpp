#ifndef RESPONSE_HEADER_HPP
# define RESPONSE_HEADER_HPP

#include <iostream>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream>
#include "Method.hpp"

class ResponseHeader
{
	private:
		std::string	protocol;
		std::string	content_type;
		std::string	content_length;
		std::string	get_extension_file(std::string path);
		
	public:
		std::string	response_header;
		int		content_size;
		ResponseHeader();
		~ResponseHeader();
		void	build_response(Method &method);
};

#endif