#ifndef CONFIG_HPP
# define CONFIG_HPP

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
#include "VirtualServerConfig.hpp"
#include "Parsing.hpp"
#include "InitBlocks.hpp"
#include "ABlock.hpp"
#include <arpa/inet.h>

#define OPEN_FILE			(std::string)"Unable to open the file : '"
#define EMPTY 				(std::string)" is empty"
#define BRACKETS			(std::string)"Config file isn't closed by brackets properly"
#define SERVER_NAME			(std::string)"Duplicate server_name : '"
#define	ERROR_SYNTAX		(std::string)"Found unexpected char on server line"
#define SEMICOLON			(std::string)"Unexpected char after semicolon : '"
#define	LISTEN				(std::string)"Wrong syntax for listen. Syntax is IPV4:PORT"
#define PORT				(std::string)"' is an invalid port. Port range: 0-65535"
#define	PATH				(std::string)"Can't open file : '"
#define	LOG_LEVEL			(std::string)"Unknown log_level"
#define	TOO_MUCH_ARGS		(std::string)"too much args for this parameter"
#define WRONG_METHOD		(std::string)"Unauthorized method : '"
#define WRONG_STATUS		(std::string)"Wrong status entered as redirection. Only accept 301/302, got : '"
#define WRONG_URL			(std::string)"Wrong URL entered : '"
#define INVALID_IP			(std::string)"Invalip ip address: '"
#define NO_SERVER_BLOCK		(std::string)"No server block has been found in the config file"
#define DIRECTORY			(std::string)"Upload file path doesn't exist's '"
#define	OUT_OF_SERVER_BLOCK	(std::string)"Found a directive out of server_block"

class VirtualServerConfig;

class Parsing
{
	private:
		std::string					_fileContent;
		int							_numberOfBlocks;
		std::vector<std::string>	_blocks;
		std::vector<int>			_bracketsPos;
		bool						_inLocationBlock;
		int							_locationBlock;
		std::vector<int>			_numberOfBlocksInEachLoc;

		void			isFile(char *confPath);
		void			removeUselessLine();
		void			parseBlocks();
		void			fillVirtualServers(std::vector<VirtualServerConfig> &vServ);
		void			assignLine(std::string &line, VirtualServerConfig &vServ);
		void			checkSyntaxFile();
		void			isDuplicateServerName(std::vector<VirtualServerConfig> &vServ);

	public:

		Parsing();
		~Parsing();

		int	parseConfigFile(char *confPath, std::vector<VirtualServerConfig> &vServ);
};

#endif
