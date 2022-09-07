#include "Setter.hpp"

Setter::Setter() : _inLocationBlock(false), _locationBlock(-1)
{
	_mapPtr["listen"] = &Setter::setListen;
	_mapPtr["methods"] = &Setter::setMethod;
	_mapPtr["server_name"] = &Setter::setServerName;
	_mapPtr["error_log"] = &Setter::setLogs;
	_mapPtr["access_log"] = &Setter::setLogs;
	_mapPtr["location"] = &Setter::setLocation;
	_mapPtr["index"] = &Setter::setRootIndex;
	_mapPtr["root"] = &Setter::setRootIndex;
	_mapPtr["client_max_body_size"] = &Setter::setMaxBodySize;
	_mapPtr["return"] = &Setter::setReturn;
	_mapPtr["autoindex"] = &Setter::setAutoIndex;
}

Setter::~Setter()
{

}

void	Setter::assignLine(std::string &line, VirtualServerConfig &vServ, bool inLocationBlock, int locationBlock)
{
	std::map<std::string, void(Setter::*)(VirtualServerConfig &vServ)>::iterator it;

	initPrivateValues(line, inLocationBlock, locationBlock);
	it = _mapPtr.find(_keyWord);
	if (it != _mapPtr.end())
		(this->*it->second)(vServ);
	_streamLine.clear();
}

void	Setter::initPrivateValues(std::string &line, bool inLocationBlock, int locationBlock)
{
	_streamLine.str(line);
	_streamLine >> _keyWord;
	_argsCount = countArgs(line);
	_inLocationBlock = inLocationBlock;
	_locationBlock = locationBlock;
}

unsigned int	Setter::countArgs(std::string line) const
{
	std::istringstream	stream(line);
	unsigned int	i = 0;

	while (stream.eof() == false)
	{
		stream >> line;
		i++;
	}
	if (((_keyWord == "listen" || _keyWord == "index" || _keyWord == "root"
		|| _keyWord == "autoindex" || _keyWord == "client_max_body_size"
		|| _keyWord == "server") && i == 2) ||	((_keyWord == "error_log" 
		|| _keyWord == "access_log" || _keyWord == "return") && i == 3) ||
		(_keyWord == "methods" && (i >= 2 && i <= 4)) || (_keyWord == "location"
		&& (i >= 3 && i <= 4)) || (_keyWord == "server_name" && i >= 2))
		return (i);
	throw(_keyWord + " : " + TOO_MUCH_ARGS);
}

void	Setter::setLocation(VirtualServerConfig &vServ)
{
	if (_argsCount == 4)
	{
		_streamLine >> _keyWord;
		vServ.loc[_locationBlock].setLocationModifier(_keyWord);
	}
	_streamLine >> _keyWord;
	vServ.loc[_locationBlock].setLocationPath(_keyWord);
}

void	Setter::setMaxBodySize(VirtualServerConfig &vServ)
{
	unsigned int	size;

	_streamLine >> _keyWord;
	size = atol(_keyWord.c_str()) * MEGABYTE;
	vServ.setMaxBodySize(size);
}

void	Setter::setReturn(VirtualServerConfig &vServ)
{
	std::vector<std::string>	node;
	
	_streamLine >> _keyWord;
	if (_keyWord.compare("302") != 0 && _keyWord.compare("301") != 0)
		throw(WRONG_STATUS + _keyWord + "'");
	node.push_back(_keyWord);
	_streamLine >> _keyWord;
	node.push_back(_keyWord);
	_inLocationBlock == true ? vServ.loc[_locationBlock].setReturn(node) : vServ.setReturn(node);
}

void	Setter::setMethod(VirtualServerConfig &vServ)
{
	while (_streamLine.eof() == false)
	{
		_streamLine >> _keyWord;
		if (_keyWord.compare("GET") == 0)
			_inLocationBlock == true ? vServ.loc[_locationBlock].setMethodGet(true) : vServ.setMethodGet(true);
		else if (_keyWord.compare("POST") == 0)
			_inLocationBlock == true ? vServ.loc[_locationBlock].setMethodPost(true) : vServ.setMethodPost(true);
		else if (_keyWord.compare("DELETE") == 0)
			_inLocationBlock == true ? vServ.loc[_locationBlock].setMethodDelete(true) : vServ.setMethodDelete(true);
		else
			throw (WRONG_METHOD + _keyWord + "'");
	}
}

void	Setter::setAutoIndex(VirtualServerConfig &vServ)
{
	_streamLine >> _keyWord;
	if (_keyWord.compare("on") == 0)
		_inLocationBlock == true ? vServ.loc[_locationBlock].setAutoIndex(true) : vServ.setAutoIndex(true);
}

void	Setter::setLogs(VirtualServerConfig &vServ)
{
	std::fstream				file;
	std::vector<std::string>	node;

	_streamLine >> _keyWord;
	file.open(_keyWord.c_str());
	if (file.is_open() == false)
		throw(PATH + _keyWord + "'");
	node.push_back(_keyWord);
	_streamLine >> _keyWord;
	if (_keyWord.compare("high") != 0 && _keyWord.compare("low") != 0)
		throw(_keyWord + " : " + LOG_LEVEL);
	node.push_back(_keyWord);
	if ((_streamLine.str().find("access_log")) != std::string::npos)
		vServ.setAccessLog(node);
	else
		vServ.setErrorLog(node);
	file.close();
}

void	Setter::setRootIndex(VirtualServerConfig &vServ)
{
	std::fstream	file;

	_streamLine >> _keyWord;
	file.open(_keyWord.c_str(), std::ios::out);
	if (file.is_open() == false)
		throw(PATH + _keyWord + "'");
	if (_streamLine.str().find("index") != std::string::npos)
		_inLocationBlock == true ? vServ.loc[_locationBlock].setIndex(_keyWord) : vServ.setIndex(_keyWord);
	else
		_inLocationBlock == true ? vServ.loc[_locationBlock].setRoot(_keyWord) : vServ.setRoot(_keyWord);
	file.close();
}

void	Setter::setListen(VirtualServerConfig &vServ)
{
	size_t						pos;
	int							port;

	if ((pos = _streamLine.str().find_first_of(':') == std::string::npos))
		throw(LISTEN);
	_streamLine >> _keyWord;
	if (inet_addr(_keyWord.substr(0, _keyWord.find_first_of(':')).c_str()) == (unsigned int)-1)
		throw(INVALID_IP + _keyWord.substr(0, _keyWord.find_first_of(':')) + "'");
	port = atoi(_keyWord.substr(_keyWord.find_first_not_of(':') + 1).c_str());
	if (port > 65535 || port < 0)
		throw("'" + _keyWord + PORT);
	vServ.setIp(_keyWord.substr(0, _keyWord.find_first_of(':')));
	vServ.setPort(_keyWord.substr(_keyWord.find_first_of(':') + 1));
}

void	Setter::setServerName(VirtualServerConfig &vServ)
{
	std::vector<std::string>	newNode;

	while (_streamLine.eof() == false)
	{
		_streamLine >> _keyWord;
		newNode.push_back(_keyWord);
	}
	vServ.setServerName(newNode);		
}