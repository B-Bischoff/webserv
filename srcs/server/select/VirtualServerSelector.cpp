#include "VirtualServerSelector.hpp"

VirtualServerSelector::VirtualServerSelector(std::map<int, VirtualServer>& servers, const RequestHeader& request)
	: _request(request), _hostName(""), _port(0)
{
	parsePortAndHost();
	convertMapToVector(servers);
}

void VirtualServerSelector::parsePortAndHost()
{
	std::string requestHost = _request.getField("Host");

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
		std::cout << matchingServers[0].getNames()[0] << " " << matchingServers[0].getIp()<<":"<<matchingServers[0].getPort()<<std::endl;
		return matchingServers[0].getServerSocket();
	}

	std::vector<VirtualServer> serverList;
	if (matchingServers.size() == 0) // Find from all possible servers
		serverList = _servers;
	else
		serverList = matchingServers;

	int nameComparisonReturn = compareToName(serverList);
	
	if (nameComparisonReturn != 0)
		return nameComparisonReturn;

	return compareToDefault(serverList);
}

std::vector<VirtualServer> VirtualServerSelector::compareToIpAndPort()
{
	std::vector<VirtualServer> matchingServers;

	for (int i = 0; i < static_cast<int>(_servers.size()); i++)
	{
		if (_servers[i].getIp() == "0.0.0.0") // Don't evalute default IP
			continue;
		if (_hostName == _servers[i].getIp() && _port == _servers[i].getPort())
			matchingServers.push_back(_servers[i]);
	}

	return matchingServers;
}

int VirtualServerSelector::compareToName(std::vector<VirtualServer>& servers)
{
	int exactNameReturn = compareToExactName(servers);
	if (exactNameReturn != 0)
	{
		std::cout << "Found by exact name" << std::endl;
		return exactNameReturn;
	}

	int wildcardsNameReturn = compareToWildcardsName(servers);
	if (wildcardsNameReturn != 0)
	{
		std::cout << "Found by wildcards" << std::endl;
		return wildcardsNameReturn;
	}

	return 0;
}

int VirtualServerSelector::compareToExactName(std::vector<VirtualServer>& servers)
{
	for (int i = 0; i < static_cast<int>(servers.size()); i++)
	{
		if (_servers[i].getIp() == "0.0.0.0") // Don't evalute default IP
			continue;
		
		if (_port == servers[i].getPort())
		{
			for (int j = 0; j < static_cast<int>(_servers[i].getNames().size()); j++)
			{
				if (_hostName == servers[i].getNames()[j])
				{
					std::cout << "SERVER NAME" << std::endl;
					return servers[i].getServerSocket();
				}
			}
		}
	}

	return 0;
}

int VirtualServerSelector::compareToWildcardsName(std::vector<VirtualServer>& servers)
{
	for (int i = 0; i < static_cast<int>(servers.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(servers[i].getNames().size()); j++)
		{
			if (servers[i].getNames()[j].find("*") != std::string::npos)
			{
				if (compareWildcard(servers[i].getNames()[j], _hostName) == true)	
					return servers[i].getServerSocket();
			}
		}
	}

	return 0;
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
				std::cout << servers[i].getNames()[0] << " " << servers[i].getIp()<<":"<<servers[i].getPort()<<std::endl;
				return servers[i].getServerSocket();
			}
			if (servers[i].getIp() == _hostName)
				return servers[i].getServerSocket();
		}
	}

	std::cout << servers[serverIndex].getNames()[0] << " " << servers[serverIndex].getIp()<<":"<<servers[serverIndex].getPort()<<std::endl;
	return defaultServerSocket;
}

bool VirtualServerSelector::compareWildcard(const std::string& reference, const std::string& target)
{
	std::vector<std::string> splittedReference;
	if (reference.find_first_not_of('*') == std::string::npos) // reference only contains '*'
		return true;
	int i = reference.find_first_not_of('*');
	while (i < static_cast<int>(reference.length()))
	{
		std::string str;
		int end;
		if (reference.find('*', i) == std::string::npos)
			end = reference.length();
		else
			end = reference.find('*', i);
		str = reference.substr(i, end - i);
		if (str.empty() == false) // consecutive wildcards can create empty str
			splittedReference.push_back(str);
		i = end + 1;
	}

	bool startsWithWildcard = reference[0] == '*';
	bool endsWithWildcard = reference[reference.length() - 1] == '*';
	if (startsWithWildcard == false)
	{
		std::string str = target.substr(0, splittedReference[0].length());
		if (str != splittedReference[0])
			return false;
	}
	if (endsWithWildcard == false)
	{
		const std::string& str = splittedReference[splittedReference.size() - 1];
		if (target.find(str.c_str(), target.length() - str.length(), str.length()) == std::string::npos)
			return false;
	}

	int previousMatch = 0;
	for (int i = 0; i < (int)splittedReference.size(); i++)
	{
		if (target.find(splittedReference[i], previousMatch) != std::string::npos)
			previousMatch = target.find(splittedReference[i], previousMatch) + splittedReference[i].length();
		else
			return false;
	}

	return true;
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
