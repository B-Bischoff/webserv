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
	if (isChunkedRequest(header))
		return receiveChunkedRequestBody(socket, buffer, maxSize);
	else
		return receiveStandardRequestBody(socket, buffer, header, maxSize);;
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

		std::cout << "chunkLength: " << chunkLength << std::endl;

		char line[chunkLength + 2]; // "+ 2" stands for "\r\n" at the end

		int nbytes = 1;
		int bytesRead = 0;
		while (nbytes > 0 && bytesRead < chunkLength)
		{
			nbytes = recv(socket, &line, chunkLength + 2, 0);

			buffer += line;
			bytesRead += nbytes;
		}
		if (nbytes == -1)
			return nbytes; // Error occured
		if (bytesRead > maxSize)
			return -1; // Throw specific error code

		totalBytesRead += bytesRead;
	}

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
	std::cout << "===== IN " << std::endl;
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
	std::cout << "===== OUT " << std::endl;
	
	return bytesToRead;
}

bool SocketCommunicator::isChunkedRequest(const RequestHeader& header)
{
	return (header.getField("Transfer-Encoding") == "chunked");
}

