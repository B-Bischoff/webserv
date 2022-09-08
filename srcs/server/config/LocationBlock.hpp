#ifndef LOCATION_SERVER_BLOCK_HPP
# define LOCATION_SERVER_BLOCK_HPP

#include "VirtualServerConfig.hpp"
#include "ABlock.hpp"

typedef std::map <std::string, std::vector <std::string> > Serv;

class LocationBlock : public ABlock
{
	private:

	public:
		LocationBlock();
		~LocationBlock();
};

#endif