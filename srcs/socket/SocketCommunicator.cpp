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

int SocketCommunicator::receiveRequestBody(const int& socket, Client& client, const long& maxSize)
{
	int retValue;
	if (isChunkedRequest(client.request))
		retValue = receiveChunkedRequestBody(socket, client.body, maxSize);
	else
		retValue = receiveStandardRequestBody(socket, client, maxSize);;

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

int SocketCommunicator::receiveStandardRequestBody(const int& socket, Client& client, const long& maxSize)
{
	size_t bytesToRead = atol(client.request.getField("Content-Length").c_str());
	if (bytesToRead <= 0)
		throw (STATUS_411);
	if (bytesToRead > static_cast<size_t>(maxSize))
		throw (STATUS_413);

	bytesToRead -= client.body.length();

	if (bytesToRead > 32768)
		bytesToRead = 32768;

	char	temp[32768];
	memset(temp, 0, 32768);
	size_t n = recv(socket, temp, bytesToRead, 0);
	if (n > 0)
		client.body.append(temp, n);

	client.bytesRead += n;


	//return atol(header.getField("Content-Length").c_str()) - buffer.length();
	return atol(client.request.getField("Content-Length").c_str()) - client.bytesRead;
}

bool SocketCommunicator::isChunkedRequest(const RequestHeader& header)
{
	return (header.getField("Transfer-Encoding") == "chunked");
}

int SocketCommunicator::sendResponse(const int& socket, Client& client)
{
	size_t SIZE = 32768;

	std::string str = client.response.response_header.substr(client.bytesSent, SIZE);

	long n = send(socket, str.c_str(), str.size(), 0);
	//std::cout << n << "/" << length << std::endl;
	if (n <= 0)
	{
		std::cout << "error" << std::endl;
		//perror("SEND");
		return -1;
	}
	else
		client.bytesSent += n;
	return client.response.response_header.length() - client.bytesSent;
}
