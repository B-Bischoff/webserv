#ifndef VIRTUAL_SERVER_SELECTOR_HPP
# define VIRTUAL_SERVER_SELECTOR_HPP

#include <iostream>
#include <vector>
#include "VirtualServer.hpp"
#include "RequestHeader.hpp"

class VirtualServerSelector
{
	private:
		const std::vector<VirtualServer>& _servers;
		const RequestHeader& _request;

		std::string _hostName;
		unsigned int _port;

		void parsePortAndHost();
		std::vector<VirtualServer> compareToIpAndPort();
		std::vector<VirtualServer> compareToNameAndPort();

	public:
		VirtualServerSelector(const std::vector<VirtualServer>& servers, const RequestHeader& request);

		const VirtualServer& selectServerFromRequest();
};

#endif
