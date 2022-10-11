#ifndef REQUEST_HEADER_HPP
# define REQUEST_HEADER_HPP

#include <iostream>
#include <istream>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <map>
#include <vector>

#include "StatusCode.hpp"

#define MAX_BODY_SIZE 30000
#define ROOT_PATH "./pages"

struct BodyData {
	std::map<std::string, std::string> metadata;
	std::string content;
};

class RequestHeader
{
	private:
		std::map<std::string, std::string> _fields;
		std::vector<BodyData> _body;

		void parseMethodPathAndVersion(std::string& line);
		void parseField(std::string& line);

		void parseMultipartEnctype(std::string& body);
		const std::string parseBoundary();
		bool parseMetadatas(const std::string& line);
		std::string parseBodyContent(std::istringstream& ss, const std::string& boundary);

		void removeWhiteSpaces(std::string& str);

	public:
		std::string getField(const std::string& field) const;

		void parseRequestHeader(std::string& header);
		void parseRequestBody(std::string& body);
};

#endif
