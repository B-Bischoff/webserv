#ifndef VIRTUAL_SERVER_CONFIG_HPP
# define VIRTUAL_SERVER_CONFIG_HPP

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <string>
#include <signal.h>
#include <vector>
#include <sstream>
#include <iterator>
#include <map>
#include <arpa/inet.h>
#include "Parsing.hpp"
#include "LocationBlock.hpp"
#include "ABlock.hpp"

class LocationBlock;

typedef std::map <std::string, std::vector <std::string> > Serv;

class VirtualServerConfig : public ABlock
{
	private:
		unsigned int		_client_max_body_size;
		std::vector<int>	_errorStatus;

	public:
		std::vector<LocationBlock>	loc;
		VirtualServerConfig();
		~VirtualServerConfig();

		void						setPort(std::string ip);
		unsigned int				getPort(void) const;

		void						setMaxBodySize(unsigned int size);
		unsigned int				getMaxBodySize(void) const;

		void						setErrorStatus(std::vector<int> values);

		std::vector<int>			getErrorStatus(void) const;
	

};

#endif
