#ifndef SOCKET_COMMUNICATOR_HPP
# define SOCKET_COMMUNICATOR_HPP

#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>

#include "Server.hpp"
#include "StatusCode.hpp"

struct Client;

class SocketCommunicator {
private:
	static int receiveStandardRequestBody(const int& socket, std::string& buffer, const RequestHeader& header, const long& maxSize);
	static int receiveChunkedRequestBody(const int& socket, std::string& buffer, const long& maxSize);
	static int receiveChunkLength(const int& socket);
	static int convertHexaNumberInStrToInt(std::string& str);

	static bool isChunkedRequest(const RequestHeader& header);

public:
	static int receiveRequestHeader(const int& socket, std::string& buffer);
	static int receiveRequestBody(const int& socket, std::string& buffer, const RequestHeader& header, const long& maxSize);
	static int sendResponse(const int& socket, Client& client);
};

#endif
