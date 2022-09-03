#include "Setter.hpp"

Setter::Setter() : _inLocationBlock(false), _locationBlock(-1)
{

}

Setter::~Setter()
{

}

void	Setter::assignLine(std::string &line, VirtualServerConfig &vServ, bool inLocationBlock, int locationBlock)
{
	std::istringstream	streamLine(line);
	unsigned int		args = countArgs(line);
	_inLocationBlock = inLocationBlock;
	_locationBlock = locationBlock;

	if (line.find("server ") != std::string::npos)
	{
		if (args != 2)
			throw(ERROR_SYNTAX);
	}
	else if (line.find("location") != std::string::npos)
		setLocation(streamLine, vServ, args);
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

unsigned int	Setter::countArgs(std::string line) const
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

void	Setter::setLocation(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args)
{
	std::string	tmp;

	streamLine >> tmp;
	if (args < 3 || args > 4)
		throw(tmp + " : " + TOO_MUCH_ARGS);
	if (args == 4)
	{
		streamLine >> tmp;
		vServ.loc[_locationBlock].setLocationModifier(tmp);
	}
	streamLine >> tmp;
	vServ.loc[_locationBlock].setLocationPath(tmp);
}

void	Setter::setMaxBodySize(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args)
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

void	Setter::setReturn(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args)
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
	_inLocationBlock == true ? vServ.loc[_locationBlock].setReturn(node) : vServ.setReturn(node);
}

void	Setter::setMethod(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args)
{
	std::string	tmp;
	streamLine >> tmp;
	if (args < 2 || args > 4)
		throw(tmp + " : " + TOO_MUCH_ARGS);
	while (streamLine.eof() == false)
	{
		streamLine >> tmp;
		if (tmp.compare("GET") == 0)
			_inLocationBlock == true ? vServ.loc[_locationBlock].setMethodGet(true) : vServ.setMethodGet(true);
		else if (tmp.compare("POST") == 0)
			_inLocationBlock == true ? vServ.loc[_locationBlock].setMethodPost(true) : vServ.setMethodPost(true);
		else if (tmp.compare("DELETE") == 0)
			_inLocationBlock == true ? vServ.loc[_locationBlock].setMethodDelete(true) : vServ.setMethodDelete(true);
		else
			throw (WRONG_METHOD + tmp + "'");
	}
}

void	Setter::setAutoIndex(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args)
{
	std::string	tmp;

	streamLine >> tmp;
	if (args != 2)
		throw(tmp + " : " + TOO_MUCH_ARGS);
	streamLine >> tmp;
	if (tmp.compare("on") == 0)
		_inLocationBlock == true ? vServ.loc[_locationBlock].setAutoIndex(true) : vServ.setAutoIndex(true);
}

void	Setter::setLogs(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args)
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

void	Setter::setRootIndex(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args)
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
		_inLocationBlock == true ? vServ.loc[_locationBlock].setIndex(tmp) : vServ.setIndex(tmp);
	else
		_inLocationBlock == true ? vServ.loc[_locationBlock].setRoot(tmp) : vServ.setRoot(tmp);
	file.close();
}

void	Setter::setListen(std::istringstream &streamLine, VirtualServerConfig &vServ, unsigned int args)
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

void	Setter::setServerName(std::string &line, VirtualServerConfig &vServ, unsigned int args)
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