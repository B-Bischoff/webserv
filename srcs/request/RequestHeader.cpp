#include "RequestHeader.hpp"

void	RequestHeader::parseRequestHeader(std::string& header)
{
	std::istringstream tmp(header);
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

void RequestHeader::parseRequestBody(std::string& body)
{
	if (getField("Content-Type").find("boundary") != std::string::npos)
		parseMultipartEnctype(body);
}

void RequestHeader::parseMultipartEnctype(std::string& body)
{
	std::cout << "============= BODY PARSING =============" << std::endl;

	const std::string boundary = parseBoundary();

	std::istringstream ss(body);
	std::string line;

	std::getline(ss, line); // Skip entry boundary


	bool parseMetadata = true;
	_body.push_back(BodyData());
	while (std::getline(ss, line))
	{
		line += '\n';
		if (line == "\r\n") // Body starts
			parseMetadata = false;

		if (parseMetadata)
			parseMetadata = parseMetadatas(line);
		else
		{
			_body[_body.size() - 1].content = parseBodyContent(ss, boundary);
			parseMetadata = true;
			_body.push_back(BodyData());
		}
	}
	_body.pop_back(); // Remove last unused body data added

	std::cout << "============= BODY PARSING RECAP =============" << std::endl;
	
	for (int i = 0; i < (int)_body.size(); i++)
	{
		for(std::map<std::string, std::string>::iterator it = _body[i].metadata.begin(); it != _body[i].metadata.end(); ++it)
			std::cout << it->first << "|" << it->second << "\n";
		std::cout << "------------------" << std::endl;
		std::cout << _body[i].content << std::endl;

	}

	std::cout << "============= BODY PARSING END =============" << std::endl;
}

const std::string RequestHeader::parseBoundary()
{
	std::string contentTypeField = getField("Content-Type");
	int boundaryStart = contentTypeField.find("=") + 1; // +1 is used to get rid of the '='
	std::string boundary = contentTypeField.substr(boundaryStart, contentTypeField.length());

	return boundary;
}

bool RequestHeader::parseMetadatas(const std::string& line)
{
	std::string key;
	std::string value;
	int i = 0;

	while (line[i] != ':')
		key += line[i++];
	i += 2; // Jump space
	while (line[i] != ';' && line[i] != '\r')
		value += line[i++];
	
	std::cout << "Key:" << key << "|Value:" << value << "|" << std::endl;
	_body[_body.size() - 1].metadata[key] = value;

	if (line[i] == '\r') // no more metadata to read
	{
		return false;
	}

	i += 2;
	key.clear();
	value.clear();

	// Read additionnal metadatas
	while (line[i] != '\r')
	{
		while (line[i] != '=')
			key += line[i++];
		i += 2;
		while (line[i] != '"')
			value += line[i++];
		i++;
		if (line[i] != '\r')
			i += 2;
		std::cout << "Key:" << key << "|Value:" << value << "|" << std::endl;
		_body[_body.size() - 1].metadata[key] = value;
		key.clear();
		value.clear();
	}
	return true;
}

std::string RequestHeader::parseBodyContent(std::istringstream& ss, const std::string& boundary)
{
	std::string line;
	std::string bodyContent;
	while (std::getline(ss, line) && line.find(boundary) == std::string::npos)
	{
		if (line != "\r")
			bodyContent += line + '\n';
	}
	std::cout << "body content>>" << bodyContent << "<<" << std::endl;
	return bodyContent;
}

std::string RequestHeader::getField(const std::string& field) const
{
	try {
		return _fields.at(field);
	}
	catch (std::out_of_range& e) {
		std::string empty;
		return empty;
	}
}

void RequestHeader::removeWhiteSpaces(std::string& str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (isprint(str[i]) == false)
			str.erase(i);
	}
}
