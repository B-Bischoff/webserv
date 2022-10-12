#ifndef SOCKET_COMMUNICATOR_HPP
# define SOCKET_COMMUNICATOR_HPP

#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <cstdlib>

#include "RequestHeader.hpp"
#include "StatusCode.hpp"

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
};

#endif
