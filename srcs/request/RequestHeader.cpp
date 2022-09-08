#include "RequestHeader.hpp"

void	RequestHeader::readRequest(std::string& request)
{
	std::istringstream tmp(request);
	std::string line;

	std::getline(tmp, line);
	parseMethodPathAndVersion(line);

	while (std::getline(tmp, line))
	{
		parseField(line);
	}
	
	// Debug: Print map content
	/*
	std::cout << "------------------" << std::endl;
	for(std::map<std::string, std::string>::iterator it = _fields.begin(); it != _fields.end(); ++it)
		std::cout << it->first << "|" << it->second << "\n";
	std::cout << "------------------" << std::endl;
	*/
}

void RequestHeader::parseMethodPathAndVersion(std::string& line)
{
	std::stringstream stream;

	stream << line;

	stream >> _fields["Method"];
	stream >> _fields["Path"];
	stream >> _fields["Version"];
}

void RequestHeader::parseField(std::string& line)
{
	std::string key, value;

	removeWhiteSpaces(line);

	if (line.empty())
		return;
	
	key = line.substr(0, line.find(':'));
	value = line.substr(line.find(':') + 2, line.length() - 1);

	_fields[key] = value;
}

void RequestHeader::removeWhiteSpaces(std::string& str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (isprint(str[i]) == false)
			str.erase(i);
	}
}

std::string RequestHeader::getField(const std::string& field) const
{
	try {
		return _fields.at(field);
	}
	catch (std::out_of_range) {
		std::string empty;
		return empty;
	}
}
