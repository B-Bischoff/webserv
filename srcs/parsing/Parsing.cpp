#include "Parsing.hpp"
static void	printServer(std::vector<VirtualServerConfig> &vServ, int size, std::vector<int> _locationBlock);

Parsing::Parsing()
{
	_numberOfBlocks = 0;
	_locationBlock = -1;
	_inLocationBlock = false;
}

Parsing::~Parsing()
{

}

void	Parsing::fillVirtualServers(std::vector<VirtualServerConfig> &vServ)
{
	std::string			line;
	VirtualServerConfig	newNode;
	LocationBlock		newNodeLoc;
	Setter				setter;

	for (int i = 0; i < _numberOfBlocks; i++)
	{
		vServ.push_back(newNode);
		std::istringstream	blockContent(_blocks[i]);
		while (std::getline(blockContent, line).eof() == false)
		{
			if (line.find("}") != std::string::npos)
				_inLocationBlock = false;
			else if (line.find("location") != std::string::npos)
			{
				_locationBlock++;
				vServ[i].loc.push_back(newNodeLoc);
				setter.assignLine(line, vServ[i], _inLocationBlock, _locationBlock);
				_inLocationBlock = true;
			}
			else
				setter.assignLine(line, vServ[i], _inLocationBlock, _locationBlock);
		}
		_numberOfBlocksInEachLoc.push_back(_locationBlock + 1);
		_locationBlock = -1;
	}
}

void	Parsing::parseBlocks()
{
	int	start = 0;

	for (int i = 0; i < _numberOfBlocks; i++)
	{
		_blocks.push_back(_fileContent.substr(start, _bracketsPos[i] - start));
		start = _bracketsPos[i];
		_blocks[i] = _blocks[i].substr(0, _blocks[i].find_last_not_of(" "));
		for (size_t n = 0; n < _blocks[i].length(); n++)
		{
			if (_blocks[i][n] == ';')
				_blocks[i].erase(n, 1);
		}
	}
}

void	Parsing::removeUselessLine()
{
	std::istringstream	fileContent(_fileContent);
	std::string	dst;
	std::string	line;
	bool		serverBlock = false;

	while (std::getline(fileContent, line))
	{
		if (line.find("server ") != std::string::npos)
			serverBlock = true;
		if(line.find_first_not_of(" \t") != std::string::npos
			&& line.empty() == false && line[0] != '#')
		{
			dst += line.substr(line.find_first_not_of(" \t"), (line.find_first_of("#") - line.find_first_not_of(" \t")));
			dst.erase(dst.find_last_not_of(' ') + 1);
			dst += '\n';
		}
	}
	if (serverBlock == false)
		throw (NO_SERVER_BLOCK);
	_fileContent = dst;
}

void	Parsing::checkSyntaxFile()
{
	int	server = 0, loc = 0, size = _fileContent.length();

	for (int i = 0; i < size; i++)
	{
		if (_fileContent[i] == '{')
		{
			if (!server)
				server++;
			else
				loc++;
		}
		else if (_fileContent[i] == '}')
		{
			if (loc)
				loc--;
			else if (server)
			{
				server--;
				_numberOfBlocks++;
				_bracketsPos.push_back(i);
			}
			else
				throw(BRACKETS);
		}
		if (_fileContent[i] == ';' && _fileContent[i + 1] != '\n')
			throw(SEMICOLON + _fileContent[i + 1] + "'");
	}
}

void	Parsing::isFile(char *confPath)
{
	std::ifstream	openFile;

	openFile.open(confPath);
	if (openFile.is_open() == false)
		throw (OPEN_FILE + confPath + "'");
	_fileContent.assign((std::istreambuf_iterator<char>(openFile)), (std::istreambuf_iterator<char>()));
	if (_fileContent.empty() == true)
		throw(confPath + EMPTY);
	openFile.close();
}

void	Parsing::isDuplicateServerName(std::vector<VirtualServerConfig> &vServ)
{
	std::vector<std::string>::const_iterator it;
	std::vector<std::string>::const_iterator itNext;
	std::vector<std::string>::const_iterator end;
	std::vector<std::string>::const_iterator endNext;

	for (int i = 0; i < _numberOfBlocks - 1; i++)
	{
		end = vServ[i].getServerName().end();
		for (it = vServ[i].getServerName().begin(); it != end; it ++)
		{
			for (int j = i + 1; j < _numberOfBlocks; j++)
			{
				endNext = vServ[j].getServerName().end();
				for (itNext = vServ[j].getServerName().begin(); itNext != endNext; itNext++)
				{
					if (*itNext == *it)
						throw (SERVER_NAME + *it + "'");
				}
			}
		}
	}
}


int	Parsing::parseConfigFile(char *confPath, std::vector<VirtualServerConfig> &vServ)
{
	try
	{
		isFile(confPath);
		removeUselessLine();
		checkSyntaxFile();
		parseBlocks();
		for (int i = 0; i < _numberOfBlocks; i++)
		{
			std::cout << _blocks[i] << std::endl;
		}
		fillVirtualServers(vServ);
		isDuplicateServerName(vServ);
	}
	catch(const std::string& e)
	{
		std::cerr << e << std::endl;
		return (-1);
	}
	printServer(vServ, _numberOfBlocks, _numberOfBlocksInEachLoc);
	return (0);
}











static void	printVector(std::vector<std::string> vec)
{
	for(std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
	{
 	   std::cout << *it << " ";
	}
	std::cout << std::endl;
}


static void	printServer(std::vector<VirtualServerConfig> &vServ, int size, std::vector<int> _locationBlock)
{
	for (int i = 0; i < size; i++)
	{
		std::cout << "BLOCK_SERVER " << i << std::endl;
		std::cout  << "Ip:\t\t"  << vServ[i].getIp() << ":" << vServ[i].getPort() << std::endl;
		std::cout << "server_name:\t";
		printVector(vServ[i].getServerName());
		std::cout << "access_log:\t";
		printVector(vServ[i].getAccessLog());
		std::cout << "error_log:\t";
		printVector(vServ[i].getErrorLog());
		std::cout << "root:\t\t" << vServ[i].getRoot() << std::endl;
		std::cout << "index\t\t" << vServ[i].getIndex() << std::endl;
		std::cout << "return:\t\t";
		printVector(vServ[i].getReturn());
		std::cout << "method:\t\t" << "GET: " << vServ[i].getMethodGet() << " " << "POST: " << vServ[i].getMethodPost() << " " << "DELETE: " << vServ[i].getMethodDelete() << std::endl;
		std::cout << "autoindex:\t" << (vServ[i].getAutoIndex() ? "yes" : "no") << std::endl;
		std::cout << "body size:\t" << vServ[i].getMaxBodySize()  << "\n" << std::endl;

		for (int j = 0; j < _locationBlock[i]; j++)
		{
			std::cout << "BLOCK_LOCATION " << j  << " in BLOCK_SERVER " << i  << "\n" << std::endl;
			std::cout << "location:\t" << "modifier: " << vServ[i].loc[j].getLocationModifier() << " path: " << vServ[i].loc[j].getLocationPath() << std::endl;
			std::cout << "method:\t\t" << "GET: " << vServ[i].loc[j].getMethodGet() << " " << "POST: " << vServ[i].loc[j].getMethodPost() << " " << "DELETE: " << vServ[i].loc[j].getMethodDelete() << std::endl;
			std::cout << "autoindex:\t" << (vServ[i].loc[j].getAutoIndex() ? "yes" : "no") << std::endl;
			std::cout << "index\t\t" << vServ[i].loc[j].getIndex() << std::endl;
			std::cout << "root:\t\t" << vServ[i].loc[j].getRoot() << std::endl;
			std::cout << "return:\t\t";
			printVector(vServ[i].loc[j].getReturn());
			std::cout << std::endl;
		}
			std::cout << "-------------------------------------" << std::endl;
	}
}