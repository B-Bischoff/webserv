#include "Parsing.hpp"

Parsing::Parsing()
{
	_blockNumber = 0;
}

Parsing::~Parsing()
{

}

static void	printVector(std::vector<std::string> vec)
{
	for(std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
	{
 	   std::cout << *it << " ";
	}
	std::cout << std::endl;
}


static void	printServer(std::vector<VirtualServerConfig> vServ, int size)
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
		std::cout << "methods:\t" << vServ[i].getMethodGet() << " "  << vServ[i].getMethodPost() << " " << vServ[i].getMethodDelete() << std::endl;
		std::cout << "autoindex:\t" << (vServ[i].getAutoIndex() ? "yes" : "no") << std::endl;
		std::cout << "body size:\t" << vServ[i].getMaxBodySize() << std::endl;
		std::cout << "-------------------------------------\n" << std::endl;
	}
}

unsigned int	Parsing::countArgs(std::string line) const
{
	std::istringstream	streamLine(line);
	unsigned int	i = 0;
	std::string		tmp;

	while (streamLine.eof() == false)
	{
		streamLine >> tmp;
		i++;
	}
	return (i);
}

void	Parsing::setMaxBodySize(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args)
{
	std::string		tmp;
	unsigned int	size;
	streamLine >> tmp;
	if (args != 2)
		throw(tmp + " : " + TOO_MUCH_ARGS);
	
	streamLine >> tmp;
	size = std::stoul(tmp) * MEGABYTE;
	vServ.setMaxBodySize(size);
}

void	Parsing::setReturn(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args)
{
	const std::regex 			pattern("((http|https)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");
	std::string					tmp;
	std::vector<std::string>	node;
	
	streamLine >> tmp;
	if (args != 3)
		throw(tmp + " : " + TOO_MUCH_ARGS);
	
	streamLine >> tmp;
	if (tmp.compare("302") == 0 || tmp.compare("301") == 0)
		node.push_back(tmp);
	else
		throw(WRONG_STATUS + tmp + "'");
	streamLine >> tmp;
	if (regex_match(tmp, pattern))
		node.push_back(tmp);
	else
		throw(WRONG_URL + tmp + "'");
	vServ.setReturn(node);
}

void	Parsing::setMethod(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args)
{
	std::string	tmp;
	streamLine >> tmp;
	if (args < 2 || args > 4)
		throw(tmp + " : " + TOO_MUCH_ARGS);
	while (streamLine.eof() == false)
	{
		streamLine >> tmp;
		if (tmp.compare("GET") == 0)
			vServ.setMethodGet(tmp);
		else if (tmp.compare("POST") == 0)
			vServ.setMethodPost(tmp);
		else if (tmp.compare("DELETE") == 0)
			vServ.setMethodDelete(tmp);
		else
			throw (WRONG_METHOD + tmp + "'");
	}
}

void	Parsing::setAutoIndex(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args)
{
	std::string	tmp;

	streamLine >> tmp;
	if (args != 2)
		throw(tmp + " : " + TOO_MUCH_ARGS);
	streamLine >> tmp;
	if (tmp.compare("on") == 0)
		vServ.setAutoIndex(true);
}

void	Parsing::setLogs(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args)
{
	std::fstream				file;
	std::string					tmp;
	std::vector<std::string>	node;

	streamLine >> tmp;
	if (args != 3)
		throw(tmp + " : " + TOO_MUCH_ARGS);
	streamLine >> tmp;
	file.open(tmp.c_str());
	if (file.is_open() == false)
		throw(PATH + tmp + "'");
	node.push_back(tmp);
	streamLine >> tmp;
	if (tmp.compare("high") != 0 && tmp.compare("low") != 0)
		throw(tmp + " : " + LOG_LEVEL);
	node.push_back(tmp);
	if ((streamLine.str().find("access_log")) != std::string::npos)
		vServ.setAccessLog(node);
	else
		vServ.setErrorLog(node);
	file.close();
}

void	Parsing::setRootIndex(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args)
{
	std::string		tmp;
	std::fstream	file;

	streamLine >> tmp;
	if (args != 2)
		throw(tmp + " : " + TOO_MUCH_ARGS);
	streamLine >> tmp;
	file.open(tmp.c_str());
	if (file.is_open() == false)
		throw(PATH + tmp + "'");
	if (streamLine.str().find("index") != std::string::npos)
		vServ.setIndex(tmp);
	else
		vServ.setRoot(tmp);
	file.close();
}

void	Parsing::setListen(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args)
{
	std::string					tmp;
	size_t						pos;
	int							port;

	streamLine >> tmp;
	if (args != 2)
		throw(tmp + " : " + TOO_MUCH_ARGS);
	if ((pos = streamLine.str().find_first_of(':') == std::string::npos))
		throw(LISTEN);
	streamLine >> tmp;
	if (inet_addr(tmp.substr(0, tmp.find_first_of(':')).c_str()) == (unsigned int)-1)
		throw(INVALID_IP + tmp.substr(0, tmp.find_first_of(':')) + "'");
	port = std::stoi(tmp.substr(tmp.find_first_of(':') + 1));
	if (port > 65535 || port < 0)
		throw(LISTEN);
	vServ.setIp(tmp.substr(0, tmp.find_first_of(':')));
	vServ.setPort(tmp.substr(tmp.find_first_of(':') + 1));
}

void	Parsing::setServerName(std::string &line, VirtualServerConfig &vServ, unsigned int args)
{
	std::stringstream			tmp;
	std::vector<std::string>	toPush;
	tmp << line;
	tmp >> line;
	if (args < 2)
		throw(toPush[0] + TOO_MUCH_ARGS);

	while (tmp.eof() == false)
	{
		tmp >> line;
		toPush.push_back(line);
	}
	vServ.setServerName(toPush);		
}

void	Parsing::assignLine(std::string &line, VirtualServerConfig &vServ)
{
	std::istringstream	streamLine(line);
	unsigned int		args = countArgs(line);
	if (line.find("server ") != std::string::npos)
	{
		if (args != 2)
			throw(ERROR_SYNTAX);
	}
	else if (line.find("location") != std::string::npos)
		return;
	else if (line.find("listen") != std::string::npos)
		setListen(streamLine, vServ, args);
	else if (line.find("server_name") != std::string::npos)
		setServerName(line, vServ, args);
	else if (line.find("autoindex") != std::string::npos)
		setAutoIndex(streamLine, vServ, args);
	else if (line.find("root") != std::string::npos
				|| line.find("index") != std::string::npos)
		setRootIndex(streamLine, vServ, args);
	else if (line.find("access_log") != std::string::npos
				|| line.find("error_log") != std::string::npos)
		setLogs(streamLine, vServ, args);
	else if (line.find("method") != std::string::npos)
		setMethod(streamLine, vServ, args);
	else if (line.find("return") != std::string::npos)
		setReturn(streamLine, vServ, args);
	else if (line.find("client_max_body_size") != std::string::npos)
		setMaxBodySize(streamLine, vServ, args);
	else if (line.find("}") != std::string::npos || line.find("{") != std::string::npos)
		return;
	else
		throw(line);
}

void	Parsing::fillVirtualServers(std::vector<VirtualServerConfig> &vServ)
{
	std::string			line;
	VirtualServerConfig	newNode;

	for (int i = 0; i < _blockNumber; i++)
	{
		vServ.push_back(newNode);
		std::istringstream	blockContent(_blocks[i]);
		while (std::getline(blockContent, line).eof() == false)
		{
			if (line.find("location") != std::string::npos)
				_inLocationBlock == true;
			assignLine(line, vServ[i]);
		}
	}
}

void	Parsing::removeSemicolon()
{
	std::string::iterator	it;
	std::string::iterator	end;
	
	for (int i = 0; i < _blockNumber; i++)
	{
		end = _blocks[i].end();
		for (it = _blocks[i].begin(); it != end; it++)
		{
			if (*it == ';')
				_blocks[i].erase(it);
		}
	}
}

void	Parsing::parseBlocks(std::string &fileContent)
{
	int	start = 0;

	for (int i = 0; i < _blockNumber; i++)
	{
		_blocks.push_back(fileContent.substr(start, _bracketsPos[i] - start));
		start = _bracketsPos[i];
		_blocks[i] = _blocks[i].substr(0, _blocks[i].find_last_not_of(" "));
	}
}

std::string	Parsing::removeUselessLine(std::istringstream fileContent)
{
	std::string	dst;
	std::string	line;

	while (std::getline(fileContent, line))
	{
		if(line.find_first_not_of(" \t") != std::string::npos
			&& line.empty() == false && line[0] != '#')
		{
			dst += line.substr(line.find_first_not_of(" \t"), (line.find_first_of("#") - line.find_first_not_of(" \t")));
			dst.erase(dst.find_last_not_of(' ') + 1);
			dst += '\n';
		}
	}
	return (dst);
}

void	Parsing::checkSyntaxFile(std::string &fileContent)
{
	int	server = 0, loc = 0, size = fileContent.length();

	for (int i = 0; i < size; i++)
	{
		if (fileContent[i] == '{')
		{
			if (!server)
				server++;
			else
				loc++;
		}
		else if (fileContent[i] == '}')
		{
			if (loc)
				loc--;
			else if (server)
			{
				server--;
				_blockNumber++;
				_bracketsPos.push_back(i);
			}
			else
				throw(BRACKETS);
		}
		if (fileContent[i] == ';' && fileContent[i + 1] != '\n')
			throw(SEMICOLON + fileContent[i + 1] + "'");
	}
}

int	Parsing::parseConfigFile(char *confPath, std::vector<VirtualServerConfig> &vServ)
{
	try
	{
		_openFile.open(confPath);
		if (_openFile.is_open() == false)
			throw (OPEN_FILE + confPath + "'");
		_fileContent.assign((std::istreambuf_iterator<char>(_openFile)), (std::istreambuf_iterator<char>()));
		if (_fileContent.empty() == true)
			throw(EMPTY);
		_fileContent = removeUselessLine(std::istringstream(_fileContent));
		checkSyntaxFile(_fileContent);
		parseBlocks(_fileContent);
		removeSemicolon();
		fillVirtualServers(vServ);
	}
	catch(const std::string& e)
	{
		std::cerr << e << std::endl;
		return (-1);
	}
	printServer(vServ, _blockNumber);
	return (0);
}