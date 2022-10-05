#ifndef VIRTUAL_SERVER_SELECTOR_HPP
# define VIRTUAL_SERVER_SELECTOR_HPP

#include <iostream>
#include <vector>
#include "VirtualServer.hpp"
#include "RequestHeader.hpp"

class VirtualServerSelector
{
	private:
		std::vector<VirtualServer> _servers;
		const RequestHeader& _request;

		std::string _hostName;
		unsigned int _port;

		void parsePortAndHost();
		void convertMapToVector(std::map<int, VirtualServer>& servers);

		std::vector<VirtualServer> compareToIpAndPort();
		int compareToName(std::vector<VirtualServer>& servers);
		int compareToExactName(std::vector<VirtualServer>& servers);
		int compareToWildcardsName(std::vector<VirtualServer>& servers);
		int compareToDefault(std::vector<VirtualServer>& servers);

		bool isASpecificIp(const std::string ip) const;

	public:
		VirtualServerSelector(std::map<int, VirtualServer>& servers, const RequestHeader& request);

		static bool compareWildcard(const std::string& source, const std::string& target);

		int selectServerFromRequest();
};

#endif
