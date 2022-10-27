#include "Parsing.hpp"

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
	InitBlocks			setter;

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
	for (int i = 0; i < _numberOfBlocks; i++)
	{
		if (vServ[i].loc.size() == 0)
		{
			vServ[i].loc.push_back(newNodeLoc);
			vServ[i].setStringField("/", "location_path");
		}
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
	std::string	tmp;
	bool		serverBlock = false;

	while (std::getline(fileContent, line))
	{
		if (line.find("server ") != std::string::npos)
			serverBlock = true;
		if (line.empty() == false && line[0] != '#' && serverBlock == false)
			throw(OUT_OF_SERVER_BLOCK);
		if(line.find_first_not_of(" \t") != std::string::npos
			&& line.empty() == false && line[0] != '#')
		{
			dst = line.substr(line.find_first_not_of(" \t"), (line.find_first_of("#") - line.find_first_not_of(" \t")));
			dst.erase(dst.find_last_not_of(' ') + 1);
			if (dst.empty() == false)
			{
				dst += '\n';
				tmp += dst;
			}
		}
	}
	if (serverBlock == false)
		throw (NO_SERVER_BLOCK);
	_fileContent = tmp;
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
	if (server || loc)
		throw(BRACKETS);
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
	std::vector<std::string>	vect;

	for (int i = 0; i < _numberOfBlocks; i++)
	{
		for (size_t j = 0; j < vServ[i].getVectorField("server_name").size(); j++)
		{
			if (std::find(vect.begin(), vect.end(), vServ[i].getVectorField("server_name")[j]) != vect.end())
				throw (SERVER_NAME + vServ[i].getVectorField("server_name")[j] + "'");
			else
				vect.push_back(vServ[i].getVectorField("server_name")[j]);
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
		fillVirtualServers(vServ);
		isDuplicateServerName(vServ);
	}
	catch(const std::string& e)
	{
		std::cerr << e << std::endl;
		return (-1);
	}
	return (0);
}
