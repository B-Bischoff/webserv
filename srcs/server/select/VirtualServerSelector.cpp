#include "VirtualServerSelector.hpp"

VirtualServerSelector::VirtualServerSelector(std::map<int, VirtualServer>& servers, const RequestHeader& request)
	: _request(request), _hostName(""), _port(0)
{
	parsePortAndHost();
	convertMapToVector(servers);
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

void VirtualServerSelector::convertMapToVector(std::map<int, VirtualServer>& servers)
{
	for (std::map<int, VirtualServer>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		_servers.push_back(it->second);
	}
}

int VirtualServerSelector::selectServerFromRequest()
{
	std::vector<VirtualServer> matchingServers;

	matchingServers = compareToIpAndPort();

	if (matchingServers.size() == 1) // Found
	{
		std::cout << "EXACT IP AND PORT" << std::endl;
		std::cout << matchingServers[0].getName() << " " << matchingServers[0].getIp()<<":"<<matchingServers[0].getPort()<<std::endl;
		return matchingServers[0].getServerSocket();
	}

	if (matchingServers.size() == 0) // Find from all possible servers
	{
		std::vector<VirtualServer> allServers = _servers;
		return compareToNameOrDefault(allServers);
	}
	else
		return compareToNameOrDefault(matchingServers);
}

std::vector<VirtualServer> VirtualServerSelector::compareToIpAndPort()
{
	std::vector<VirtualServer> matchingServers;

	for (int i = 0; i < static_cast<int>(_servers.size()); i++)
	{
		if (_servers[i].getIp() == "0.0.0.0") // Don't evalute default IP
			continue;
		if (_hostName == _servers[i].getIp() && _port == _servers[i].getPort())
		{
			matchingServers.push_back(_servers[i]);
		}
	}

	return matchingServers;
}

int VirtualServerSelector::compareToNameOrDefault(std::vector<VirtualServer>& servers)
{
	std::vector<VirtualServer> matchingServers;

	matchingServers = compareToNameAndPort(servers);
	if (matchingServers.size() > 0) // Cannot be greater than 1 because servers can't have the exact same name
	{
		std::cout << matchingServers[0].getName() << " " << matchingServers[0].getIp()<<":"<<matchingServers[0].getPort()<<std::endl;
		return matchingServers[0].getServerSocket();
	}
	
	// To do: research with weird syntax (wildcards and stuff ...)
	// HERE

	return compareToDefault(servers);
}

std::vector<VirtualServer> VirtualServerSelector::compareToNameAndPort(std::vector<VirtualServer>& servers)
{
	std::vector<VirtualServer> matchingServers;

	for (int i = 0; i < static_cast<int>(servers.size()); i++)
	{
		if (_servers[i].getIp() == "0.0.0.0") // Don't evalute default IP
			continue;
		if (_hostName == servers[i].getName() && _port == servers[i].getPort())
		{
			std::cout << "SERVER NAME" << std::endl;
			matchingServers.push_back(servers[i]);
			break;
		}
	}

	return matchingServers;
}

int VirtualServerSelector::compareToDefault(std::vector<VirtualServer>& servers)
{
	std::cout << "DEFAULT SERVER" << std::endl;

	int defaultServerSocket = 0;
	int serverIndex = 0;

	for (int i = 0; i < static_cast<int>(servers.size()); i++)
	{
		if (servers[i].getPort() == _port)
		{
			if (servers[i].getIp() == "0.0.0.0" && defaultServerSocket == 0)
			{
				defaultServerSocket = servers[i].getServerSocket();
				serverIndex = i;
			}
			if (servers[i].getIp() != "0.0.0.0" && !isASpecificIp(_hostName))
			{
				std::cout << servers[i].getName() << " " << servers[i].getIp()<<":"<<servers[i].getPort()<<std::endl;
				return servers[i].getServerSocket();
			}
		}
	}

	std::cout << servers[serverIndex].getName() << " " << servers[serverIndex].getIp()<<":"<<servers[serverIndex].getPort()<<std::endl;
	return defaultServerSocket;
}

bool VirtualServerSelector::isASpecificIp(const std::string ip) const
{
	if (ip.find_first_not_of("0123456789.") != std::string::npos)
		return false;
	else if (ip == "0.0.0.0")
		return false;
	else
		return true;
}
