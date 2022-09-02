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
#include <arpa/inet.h>
#include <regex>

#define	MEGABYTE		1000000
#define OPEN_FILE		(std::string)"Unable to open the file : '"
#define EMPTY 			(std::string)"Config file is empty"
#define BRACKETS		(std::string)"Config file isn't closed by brackets properly"
#define SERVER_NAME		(std::string)"Duplicate server_name"
#define	ERROR_SYNTAX	(std::string)"Found unexpected char on server line"
#define SEMICOLON		(std::string)"Unexpected char after semicolon : '"
#define	LISTEN			(std::string)"Wrong syntax for listen. Syntax is IPV4:PORT"
#define PORT			(std::string)"Port range: 0-65535"
#define	PATH			(std::string)"Can't open file : '"
#define	LOG_LEVEL		(std::string)"Unknown log_level"
#define	TOO_MUCH_ARGS	(std::string)"too much args for this parameter"
#define WRONG_METHOD	(std::string)"Unauthorized method : '"
#define WRONG_STATUS	(std::string)"Wrong status entered as redirection. Only accept 301/302, got : '"
#define WRONG_URL		(std::string)"Wrong URL entered : '"
#define INVALID_IP		(std::string)"Invalip ip address: '"

class VirtualServerConfig;

class Parsing
{
	private:
		std::ifstream				_openFile;
		std::string					_fileContent;
		std::vector<std::string>	_blocks;
		int							_blockNumber;
		std::vector<int>			_bracketsPos;

		std::string		removeUselessLine(std::istringstream fileContent);
		void			parseBlocks(std::string &fileContent);
		void			fillVirtualServers(std::vector<VirtualServerConfig> &vServ);
		void			assignLine(std::string &line, VirtualServerConfig &vServ);
		void			checkSyntaxFile(std::string &fileContent);
		void			removeSemicolon();
		unsigned int	countArgs(std::string line) const;

		void			setListen(std::istringstream &line, VirtualServerConfig &vServ, unsigned int args);
		void			setServerName(std::string &line, VirtualServerConfig &vServ, unsigned int args);
		void			setRootIndex(std::istringstream &line, VirtualServerConfig &vServ, unsigned int args);
		void			setLogs(std::istringstream &line, VirtualServerConfig &vServ, unsigned int args);
		void			setAutoIndex(std::istringstream &line, VirtualServerConfig &vServ, unsigned int args);
		void			setMethod(std::istringstream &line, VirtualServerConfig &vServ, unsigned int args);
		void			setReturn(std::istringstream &line, VirtualServerConfig &vServ, unsigned int args);
		void			setMaxBodySize(std::istringstream &line, VirtualServerConfig &vServ, unsigned int args);

		void			setLocation(std::istringstream &line, VirtualServerConfig &vServ, unsigned int args);
	public:

		Parsing();
		~Parsing();

		int	parseConfigFile(char *confPath, std::vector<VirtualServerConfig> &vServ);
};

#endif