#ifndef INIT_BLOCKS_HPP
# define INIT_BLOCKS_HPP

#include "Parsing.hpp"
#include <sys/types.h>
#include <sys/stat.h>

class VirtualServerConfig;

class InitBlocks
{
	private:
		bool				_inLocationBlock;
		int					_locationBlock;
		unsigned int		_argsCount;
		std::string			_keyWord;
		std::istringstream	_streamLine;
		std::map<std::string, void(InitBlocks::*)(VirtualServerConfig &vServ)> _mapPtr;
		
		void			setListen(VirtualServerConfig &vServ);
		void			setServerName(VirtualServerConfig &vServ);
		void			setRootIndex(VirtualServerConfig &vServ);
		void			setLogs(VirtualServerConfig &vServ);
		void			setAutoIndex(VirtualServerConfig &vServ);
		void			setMethod(VirtualServerConfig &vServ);
		void			setReturn(VirtualServerConfig &vServ);
		void			setMaxBodySize(VirtualServerConfig &vServ);
		void			setLocation(VirtualServerConfig &vServ);
		void			setCgiPass(VirtualServerConfig &vServ);
		void			setErrorPage(VirtualServerConfig &vServ);
		void			setUploadPath(VirtualServerConfig &vServ);
		unsigned int	countArgs(std::string line) const;
		void			initPrivateValues(std::string &line, bool inLocationBlock, int locationBlock);


	public:
		InitBlocks();
		~InitBlocks();

		void	assignLine(std::string &line, VirtualServerConfig &vServ, bool inLocationBlock, int locationBlock);
};


#endif
