#ifndef SETTER_HPP
# define SETTER_HPP

#include "Parsing.hpp"

class VirtualServerConfig;

class Setter
{
	private:
		bool			_inLocationBlock;
		int				_locationBlock;
		
		void			setListen(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args);
		void			setServerName(std::string &streamLine, VirtualServerConfig &vServ, unsigned int args);
		void			setRootIndex(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args);
		void			setLogs(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args);
		void			setAutoIndex(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args);
		void			setMethod(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args);
		void			setReturn(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args);
		void			setMaxBodySize(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args);
		void			setLocation(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args);
		unsigned int	countArgs(std::string line) const;


	public:
		Setter();
		~Setter();

		void	assignLine(std::string &line, VirtualServerConfig &vServ, bool inLocationBlock, int locationBlock);
};


#endif