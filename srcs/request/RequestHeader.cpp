#include "RequestHeader.hpp"

RequestHeader::RequestHeader()
{
}

RequestHeader::~RequestHeader() 
{

}

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

	// for(std::map<std::string, std::string>::iterator it = _fields.begin(); it != _fields.end(); ++it)
	//	std::cout << it->first << "|" << it->second << "\n";
}

void RequestHeader::parseMethodPathAndVersion(std::string& line)
{
	std::stringstream stream;
	std::string method, path, version;

	stream << line;

	stream >> method;
	stream >> path;
	stream >> version;

	if (path == "/")
		path = "/index.html";

	_fields["Method"] = method;
	_fields["Path"] = ROOT_PATH + path;
	_fields["Version"] = version;
}

void RequestHeader::parseField(std::string& line)
{
	std::string key, value;
	
	key = line.substr(0, line.find(':'));
	value = line.substr(line.find(':') + 2, line.length() - 1);

	//std::cout << " Key: " << key << "|Value:" << value << "|" << std::endl;

	_fields[key] = value;
}

const std::string&	RequestHeader::getField(const std::string& field) const
{
	return _fields.at(field);
}
