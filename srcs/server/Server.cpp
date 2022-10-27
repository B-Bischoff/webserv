#include "Server.hpp"

Server::Server(const std::vector<VirtualServerConfig>& configList) : _fdmax(0)
{
	FD_ZERO(&_master); // Clear master set

	for (int i = 0; i < (int)configList.size(); i++)
		createVirtualServer(configList[i]);

	serverLoop();
}

void Server::createVirtualServer(const VirtualServerConfig& config)
{
	VirtualServer virtualServer(config); // Check virtual server creation

	_servers.insert(std::pair<int, VirtualServer>(virtualServer.getServerSocket(), virtualServer));
	addFd(virtualServer.getServerSocket(), _master);
}

void Server::addFd(const int &fd, fd_set& set)
{
	FD_SET(fd, &set);
	if (fd > _fdmax)
		_fdmax = fd;
}

void Server::removeFd(const int &fd, fd_set& set)
{
	FD_CLR(fd, &set);
	close(fd);
	if (fd == _fdmax)
		_fdmax--;
}

void Server::serverLoop()
{
    while(1)
    {
		//std::cout << std::endl << "+++++++ Waiting for new connection ++++++++" << std::endl << std::endl;

		fd_set readFds = _master;
		fd_set writeFds = createWriteFdSet();
		if (select(_fdmax + 1, &readFds, &writeFds, NULL, NULL) == -1)
		{
			std::cerr << "SELECTED FAILED QUITTING PROGRAM" << std::endl;
			exit(EXIT_FAILURE); // To remove
		}

		for (int i = 0; i <= _fdmax; i++)
		{
			if (FD_ISSET(i, &readFds))
			{
				if (isAVirtualServer(i)) // Incoming connection from new client
					acceptConnection(i);
				else // Client wants to communicate
					manageClientRequest(i);
			}
			if (FD_ISSET(i, &writeFds) && _clients[i].response.response_header != "")
			{
				respondToClient(i);
				break;
			}
		}
	}
}

fd_set Server::createWriteFdSet() const
{
	fd_set writeFds;
	FD_ZERO(&writeFds);

	std::map<int, Client>::const_iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
		if (it->second.response.response_header != "")
			FD_SET(it->first, &writeFds);

	return writeFds;
}

void Server::acceptConnection(const int& serverSocket)
{
	_newSocket = accept(serverSocket, NULL, NULL);
	if (_newSocket == -1)
		perror("accept");
	else
	{
		std::cout << "New incoming connection (fd: " << _newSocket << ")";
		std::cout << " received from fd: " << serverSocket << std::endl;
		fcntl(_newSocket, F_SETFL, O_NONBLOCK); // Set fd to non-blockant (prg will not get stuck on recv)
		addFd(_newSocket, _master);
		_clients[_newSocket].needToReceiveBody = false;
		_clients[_newSocket].virtualServerId = 0;
		_clients[_newSocket].bytesSent = 0;
		_clients[_newSocket].bytesRead = 0;
	}
}

void Server::manageClientRequest(const int& clientFd)
{
	Method method;
	try {
		if (listenClient(clientFd) == 1)
			return; // Return to select to wait request body
		method = processClientRequest(clientFd);
	}
	catch (const char* e) {
		method = manageError(clientFd, e);
	}
	catch (...) {
		method = manageError(clientFd, STATUS_500);
	}
	createClientResponseFromMethod(clientFd, method);
}

int Server::listenClient(const int& clientFd)
{
	if (_clients[clientFd].needToReceiveBody == false)
	{
		if (listenHeader(clientFd) == 1)
			return 1; // Go back to select to wait request body
	}
	else
	{
		if (listenBody(clientFd) != 0)
			return 1;
		Client& client = _clients[clientFd];
		VirtualServerConfig& config = _servers.at(_clients[clientFd].virtualServerId).getVirtualServerConfig();
		if (atol(client.request.getField("Content-Length").c_str()) > config.getMaxBodySize())
			throw (STATUS_413);
	}

	return 0;
}

int Server::listenHeader(const int& clientFd)
{
	int receiveReturn = SocketCommunicator::receiveRequestHeader(clientFd, _clients[clientFd].body);
	//std::cout << "Request Header: " << buf << std::endl;

	if (receiveReturn < 0) // Client disconnected or recv error
	{
		std::cout << "Socket: " << clientFd << " quit" << std::endl;
		removeFd(clientFd, _master);
		_clients.erase(clientFd);
		return 1;
	}
	else if (receiveReturn == 1)
		return 1;

	Client& client = _clients[clientFd];
	
	client.request.parseRequestHeader(_clients[clientFd].body);
	_clients[clientFd].body.clear();

	VirtualServerSelector selector(_servers, client.request);
	int virtualServerId = selector.selectServerFromRequest();
	if (virtualServerId == 0) // To put in a try :)
		throw(STATUS_400);
	client.virtualServerId = virtualServerId;

	if (needToReceiveBody(client.request))
	{
		client.needToReceiveBody = true;
		return 1;
	}

	return 0;
}

int Server::listenBody(const int& clientFd)
{
	int recvBodyReturn = SocketCommunicator::receiveRequestBody(clientFd, _clients[clientFd]);

	if (recvBodyReturn == -1)
	{
		removeFd(clientFd, _master);
		_clients.erase(clientFd);
		return -1;
	}
	else if (recvBodyReturn != 0)
		return 1;

	//std::cout << "Request body: " << _clients[clientFd].body << std::endl;
	_clients[clientFd].request.parseRequestBody(_clients[clientFd].body);
	return 0;
}

const Method Server::processClientRequest(const int& clientFd)
{
	Client& client = _clients[clientFd];
	client.virtualServer = _servers.at(client.virtualServerId).getVirtualServerConfig();
	client.locationBlock = LocationSelector::selectLocationBlock(client.request.getField("Path"), client.virtualServer.loc);
	Method dst;
	ManageRequest manager(client);
	dst = manager.identify();

	dst.setCloseAfterSend(client.request.getField("Connection") == "close");

	return dst;
}

Method Server::manageError(const int& clientFd, const char* e)
{
	Method method;
	ErrorStatus error;
	if (_clients[clientFd].virtualServerId == 0)
		method = error.buildError(e);
	else
	{
		VirtualServerConfig& config = _servers.at(_clients[clientFd].virtualServerId).getVirtualServerConfig();
		method = error.buildError(e, config);
	}
	return method;
}

void Server::createClientResponseFromMethod(const int& clientFd, Method& method)
{
	ResponseHeader header;

	try
	{
		header.build_response(method);
	}
	catch(const char *e)
	{
		ErrorStatus	error;
		error.buildError(e);
		header.build_response(error);
	}
	
	header.closeAfterSend = method.getCloseAfterSend();
	_clients[clientFd].response = header;
	_clients[clientFd].needToReceiveBody = false;
}

void Server::respondToClient(const int& clientFd)
{
	bool sendError = false;
	//std::cout << "Client " << clientFd << " ready to receive response" << std::endl;
	const ResponseHeader& response = _clients[clientFd].response;

	int sendReturn = SocketCommunicator::sendResponse(clientFd, _clients[clientFd]);

	if (sendReturn == -1)
	{
		std::cout << "Send error" << std::endl;
		sendError = true;
	}
	if (sendReturn <= 0) // If data fully sent or error 
	{
		_clients.erase(clientFd);
		if (response.closeAfterSend == true || sendError == true)
		{
			std::cout << "Removed client" << std::endl;
			removeFd(clientFd, _master);
		}
	}
}

bool Server::needToReceiveBody(const RequestHeader& request) const
{
	if (request.getField("Content-Length") != "")
		return true;
	else if (request.getField("Transfer-Encoding") != "")
		return true;
	else
		return false;
}

bool Server::isAVirtualServer(const int& fd) const
{
	return _servers.find(fd) != _servers.end();
}
