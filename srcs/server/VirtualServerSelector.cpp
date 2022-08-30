#include "VirtualServerSelector.hpp"

VirtualServerSelector::VirtualServerSelector(const std::vector<VirtualServer>& servers, const RequestHeader& request)
	: _servers(servers), _request(request), _hostName(""), _port(0)
{

}

const VirtualServer& VirtualServerSelector::selectServerFromRequest()
{
	parsePortAndHost();

	std::vector<VirtualServer> matchingServers;

	matchingServers = compareToIpAndPort();
	if (matchingServers.size() == 1)
		std::cout << matchingServers[0].getName() << " should process request." << std::endl;
	else if (matchingServers.size() > 1)
	{
		// Do stuff
	}

	return _servers[0];
}

void VirtualServerSelector::parsePortAndHost()
{
	std::string requestHost = _request.getHost();

	if (requestHost.find(':') != std::string::npos)
	{
		// Finding port
		std::string portInString = requestHost.substr(requestHost.find(':') + 1, requestHost.length() - 1);
		_port = strtod(portInString.c_str(), NULL);

		// Finding hostName
		_hostName = requestHost.substr(0, requestHost.find(':'));
	}
	else
		_hostName = requestHost;

	if (_port == 0) // No port was found
		_port = 80; // Set default port

	std::cout << "Hostname: " << _hostName << " | port: " << _port << std::endl;
}

std::vector<VirtualServer> VirtualServerSelector::compareToIpAndPort()
{
	std::vector<VirtualServer> matchingServers;

	for (int i = 0; i < static_cast<int>(_servers.size()); i++)
	{
		if (_hostName == _servers[i].getIp() && _port == _servers[i].getPort())
			matchingServers.push_back(_servers[i]);
	}

	return matchingServers;
}
