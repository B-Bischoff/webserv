#include "SocketCommunicator.hpp"

int SocketCommunicator::receiveRequestHeader(const int& socket, std::string& buffer)
{
	int nbytes = 1;
	char temp;

	while (nbytes > 0 && buffer.find("\r\n\r\n") == std::string::npos)
	{
		nbytes = recv(socket, &temp, 1, 0);
		if (nbytes > 0)
			buffer += temp;
	}

	return nbytes;
}

int SocketCommunicator::receiveRequestBody(const int& socket, std::string& buffer, const RequestHeader& header, const int& maxSize)
{
	int retValue;
	if (isChunkedRequest(header))
		retValue = receiveChunkedRequestBody(socket, buffer, maxSize);
	else
		retValue = receiveStandardRequestBody(socket, buffer, header, maxSize);;

	if (header.getField("Content-Type").find("boundary") != std::string::npos)
		parseBody(buffer, header);
	
	return retValue;
}

int SocketCommunicator::receiveChunkedRequestBody(const int& socket, std::string& buffer, const int& maxSize)
{
	int totalBytesRead = 0;
	int chunkLength = 1;
	
	while (totalBytesRead <= maxSize && chunkLength > 0)
	{
		chunkLength = receiveChunkLength(socket);
		if (chunkLength == -1)
			return -1; // Error occured

		int nbytes = 1;
		int bytesRead = 0;
		char temp;
		while (nbytes > 0 && bytesRead < chunkLength + 2) // "+2" is to also read "\r\n" (end of chunk indicator)
		{
			nbytes = recv(socket, &temp, 1, 0);
			if (nbytes > 0)
			{
				buffer += temp;
				bytesRead += nbytes;
				totalBytesRead += nbytes;
			}
		}
		if (nbytes == -1)
			return nbytes; // Error occured
		buffer.erase(buffer.length() - 2, 2); // Remove "\r\n" (end of chunk indicator)
	}

	if (totalBytesRead > maxSize)
		return -1; // Throw specific exception instead of -1

	return 0;
}

int SocketCommunicator::receiveChunkLength(const int& socket)
{
	std::string buffer;
	int nbytes = 1;
	char temp;

	while (nbytes > 0 && buffer.find("\r\n") == std::string::npos)
	{
		nbytes = recv(socket, &temp, 1, 0);
		if (nbytes > 0)
			buffer += temp;
	}
	if (nbytes == -1) // Error occured in recv
		return nbytes;
	else
		return convertHexaNumberInStrToInt(buffer);
}

int SocketCommunicator::convertHexaNumberInStrToInt(std::string& str)
{
	int number;

	std::stringstream ss(str);
	ss >> std::hex >> number;

	return number;
}

int SocketCommunicator::receiveStandardRequestBody(const int& socket, std::string& buffer, const RequestHeader& header, const int& maxSize)
{
	int bytesToRead = atoi(header.getField("Content-Length").c_str());
	if (bytesToRead <= 0)
		return 0;
	if (bytesToRead > maxSize) // Need to throw a specific error code
		return 0; // Change this by "throw [ERROR_CODE]"

	char temp;
	int nbytes = 1;

	while (bytesToRead > 0 && nbytes > 0)
	{
		nbytes = recv(socket, &temp, 1, 0);
		if (nbytes > 0)
			buffer += temp;
		bytesToRead -= 1;
	}
	
	return nbytes;
}

void SocketCommunicator::parseBody(std::string& buffer, const RequestHeader& header)
{
	std::cout << "============= BODY PARSING =============" << std::endl;

	// Getting boundary
	std::string contentTypeField = header.getField("Content-Type");
	int boundaryStart = contentTypeField.find("=") + 1; // +1 is used to get rid of the '='
	std::string boundary = contentTypeField.substr(boundaryStart, contentTypeField.length());

	std::cout << "boundary: " << boundary << std::endl;

	std::istringstream ss(buffer);
	std::string line;

	std::getline(ss, line); // Skip entry boundary


	bool parseMetadata = true;
	while (std::getline(ss, line))
	{
		line += '\n';

		if (parseMetadata)
		{
		
			if (line == "\r\n") // Body starts
			{
				parseMetadata = false;
				break;
			}

			std::string key;
			std::string value;
			int i = 0;

			while (line[i] != ':')
				key += line[i++];
			i += 2; // Jump space
			while (line[i] != ';' && line[i] != '\r')
				value += line[i++];
			
			std::cout << "Key:" << key << "|Value:" << value << "|" << std::endl;

			if (line[i] == '\r')
			{
				parseMetadata = false;
				break;
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
				key.clear();
				value.clear();
			}
			parseMetadata = false;
		}
		else
		{
			std::string bodyContent;
			while (std::getline(ss, line) && line.find(boundary) == std::string::npos)
			{
				if (line != "\r")
					bodyContent += line + '\n';
			}
			std::cout << "body content>>" << bodyContent << "<<" << std::endl;
			parseMetadata = true;
		}
	}



	std::cout << "============= BODY PARSING END =============" << std::endl;
}

bool SocketCommunicator::isChunkedRequest(const RequestHeader& header)
{
	return (header.getField("Transfer-Encoding") == "chunked");
}

