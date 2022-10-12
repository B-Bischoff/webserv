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

int SocketCommunicator::receiveRequestBody(const int& socket, std::string& buffer, const RequestHeader& header, const long& maxSize)
{
	int retValue;
	if (isChunkedRequest(header))
		retValue = receiveChunkedRequestBody(socket, buffer, maxSize);
	else
		retValue = receiveStandardRequestBody(socket, buffer, header, maxSize);;
	
	return retValue;
}

int SocketCommunicator::receiveChunkedRequestBody(const int& socket, std::string& buffer, const long& maxSize)
{
	long totalBytesRead = 0;
	int chunkLength = 1;
	
	while (totalBytesRead <= maxSize && chunkLength > 0)
	{
		chunkLength = receiveChunkLength(socket);
		if (chunkLength == -1)
			throw (STATUS_500);

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
			throw (STATUS_500);
		buffer.erase(buffer.length() - 2, 2); // Remove "\r\n" (end of chunk indicator)
	}

	if (totalBytesRead > maxSize)
		throw (STATUS_413);

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

int SocketCommunicator::receiveStandardRequestBody(const int& socket, std::string& buffer, const RequestHeader& header, const long& maxSize)
{
	long bytesToRead = atol(header.getField("Content-Length").c_str());
	if (bytesToRead <= 0)
		throw (STATUS_411);
	if (bytesToRead > maxSize) // Need to throw a specific error code
		throw (STATUS_413);

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

bool SocketCommunicator::isChunkedRequest(const RequestHeader& header)
{
	return (header.getField("Transfer-Encoding") == "chunked");
}
