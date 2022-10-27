#include "SocketCommunicator.hpp"

int SocketCommunicator::receiveRequestHeader(const int& socket, std::string& buffer)
{
	int nbytes = 1;
	char temp;

	nbytes = recv(socket, &temp, 1, 0);
	if (nbytes > 0)
		buffer += temp;

	if (nbytes <= 0) // Error
		return -1;
	else if (buffer.find("\r\n\r\n") == std::string::npos) // Need to go back to select
		return 1;
	return 0; // Header fully read
}

int SocketCommunicator::receiveRequestBody(const int& socket, Client& client)
{
	int retValue;
	if (isChunkedRequest(client.request))
		retValue = receiveChunkedRequestBody(socket, client.body);
	else
		retValue = receiveStandardRequestBody(socket, client);;

	return retValue;
}

int SocketCommunicator::convertHexaNumberInStrToInt(std::string& str)
{
	int number;

	std::stringstream ss(str);
	ss >> std::hex >> number;

	return number;
}

int SocketCommunicator::receiveStandardRequestBody(const int& socket, Client& client)
{
	size_t bytesToRead = atol(client.request.getField("Content-Length").c_str());
	if (bytesToRead <= 0)
		throw (STATUS_411);

	bytesToRead -= client.body.length();

	if (bytesToRead > 32768)
		bytesToRead = 32768;

	char	temp[32768];
	memset(temp, 0, 32768);
	size_t n = recv(socket, temp, bytesToRead, 0);
	if (n > 0)
		client.body.append(temp, n);

    if (n <= 0)
        return n;

	client.bytesRead += n;

	return atol(client.request.getField("Content-Length").c_str()) - client.bytesRead;
}

int SocketCommunicator::receiveChunkedRequestBody(const int& socket, std::string& buffer)
{
	long totalBytesRead = 0;
	int chunkLength = 1;
	
	while (chunkLength > 0)
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
		if (nbytes <= 0)
			throw (STATUS_500);
		buffer.erase(buffer.length() - 2, 2); // Remove "\r\n" (end of chunk indicator)
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
	if (nbytes <= 0) // Error occured in recv
		return nbytes;
	else
		return convertHexaNumberInStrToInt(buffer);
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
	if (n <= 0)
		return -1;
	else
		client.bytesSent += n;
	return client.response.response_header.length() - client.bytesSent;
}
