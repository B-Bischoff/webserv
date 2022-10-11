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
}

void Server::serverLoop()
{
    while(1)
    {
		std::cout << std::endl << "+++++++ Waiting for new connection ++++++++" << std::endl << std::endl;

		//std::cout << "Response ready: " << _clients.size() << std::endl;

		fd_set readFds = _master;
		fd_set writeFds = createWriteFdSet();
		if (select(_fdmax + 1, &readFds, &writeFds, NULL, NULL) == -1)
		{
			perror("Select");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i <= _fdmax; i++)
		{
			if (FD_ISSET(i, &readFds)) // Socket ready to communicate
			{
				if (isAVirtualServer(i)) // Incoming connection from new client
					acceptConnection(i);
				else // Client wants to communicate
					listenClient(i);
			}
			if (FD_ISSET(i, &writeFds) && _clients[i].response.response_header != "") // Send data to client
			{
				std::cout << "Client " << i << " ready to receive response" << std::endl;
				const ResponseHeader& response = _clients[i].response;
				// Make a send all method in SocketCommunicator
				std::cout << response.response_header << std::endl;
				if (send(i, response.response_header.c_str(), response.response_header.size(), 0) == -1)
				{
					std::cout << "Send error" << std::endl;
					removeFd(i, _master);
					return;
				}
				_clients.erase(i);
				if (response.closeAfterSend == true)
					removeFd(i, _master);
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
	}
}

void Server::listenClient(const int& clientFd)
{
	Method method;


	std::cout << "client " << clientFd << " wants to communicate" << std::endl;

	if (_clients[clientFd].needToReceiveBody == false)
	{
		if (listenHeader(clientFd) == 1)
			return; // Go back to select to wait request body
	}
	else
	{
		try {
			listenBody(clientFd);
		}
		catch (const char* e)
		{
			ErrorStatus error;
			method = error.buildError(e, _servers.at(_clients[clientFd].virtualServerId).getVirtualServerConfig());
			createClientResponse(clientFd, method);
			return;
		}
	}

	method = processClientRequest(clientFd);

	createClientResponse(clientFd, method);
}

int Server::listenHeader(const int& clientFd)
{
	std::string buf;
	int receiveReturn = SocketCommunicator::receiveRequestHeader(clientFd, buf);
	std::cout << "Request Header: " << buf << std::endl;

	if (receiveReturn <= 0) // Client disconnected or recv error
	{
		if (receiveReturn == 0)
			std::cout << "Socket: " << clientFd << " disconnected" << std::endl;
		else
			perror("recv");
		std::cout << "Socket: " << clientFd << " quit" << std::endl;
		removeFd(clientFd, _master);
		return 1;
	}

	Client& client = _clients[clientFd];
	
	client.request.parseRequestHeader(buf);

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

void Server::listenBody(const int& clientFd)
{
	VirtualServerConfig& config = _servers.at(_clients[clientFd].virtualServerId).getVirtualServerConfig();

	std::string requestBody;
	if (SocketCommunicator::receiveRequestBody(clientFd, requestBody, _clients[clientFd].request, config.getMaxBodySize()) == -1)
	{
		std::cout << "Recv (request body) error" << std::endl;
		throw ("ERROR");
	}

	std::cout << "Request body: " << requestBody << std::endl;
	_clients[clientFd].body = requestBody;
	_clients[clientFd].request.parseRequestBody(requestBody);
}

const Method Server::processClientRequest(const int& clientFd)
{
	Client& client = _clients[clientFd];
	VirtualServerConfig& config = _servers.at(client.virtualServerId).getVirtualServerConfig();
	LocationSelector	select;
	LocationBlock locationBlock = select.selectLocationBlock(client.request.getField("Path"), config.loc);

	Method dst;
	try {
		ManageRequest manager(config, locationBlock, client.request, client.body);
		dst = manager.identify(client.request);
	}
	catch(const char *e)
	{
		ErrorStatus error;
		dst = error.buildError(e, config);
	}

	dst.setCloseAfterSend(client.request.getField("Connection") == "close");

	std::cout << dst.getBody() << std::endl;
	return dst;
}

void Server::createClientResponse(const int& clientFd, Method& method)
{
	ResponseHeader header;

	header.build_response(method);
	header.closeAfterSend = method.getCloseAfterSend();
	_clients[clientFd].response = header;
	_clients[clientFd].needToReceiveBody = false;
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
