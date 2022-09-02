#include "LocationBlock.hpp"

LocationBlock::LocationBlock()
{
	_location["method"].push_back("");
	_location["method"].push_back("");
	_location["method"].push_back("");
	_location["index"].push_back("");
	_location["root"].push_back("");
	_location["autoindex"].push_back("no");
	_location["return"].push_back("");
	_location["location_path"].push_back("");
	_location["location_modifier"].push_back("");
}

LocationBlock::~LocationBlock()
{
	
}

void	LocationBlock::setAutoIndexLoc(bool value)
{
	_location["autoindex"][0] = (value == true ? "yes" : "no");
}

bool	LocationBlock::getAutoIndexLoc() const
{
	return ((_location.at("autoindex")[0] == "" ? false : true));
}

void	LocationBlock::setReturnLoc(std::vector<std::string> value)
{
	_location["return"] = value;
}

std::vector<std::string>	LocationBlock::getReturnLoc() const
{
	return (_location.at("return"));
}


void	LocationBlock::setRootLoc(std::string path)
{
	_location["root"][0] = path;
}

std::string	LocationBlock::getRootLoc() const
{
	return (_location.at("root")[0]);
}

void	LocationBlock::setIndexLoc(std::string path)
{
	_location["index"][0] = path;
}

std::string	LocationBlock::getIndexLoc() const
{
	return (_location.at("index")[0]);
}

void	LocationBlock::setMethodGetLoc(std::string method)
{
	_location["method"][0] = method;
}

std::string	LocationBlock::getMethodGetLoc() const
{
	return (_location.at("method")[0]);
}

void	LocationBlock::setMethodPostLoc(std::string method)
{
	_location["method"][1] = method;
}

std::string	LocationBlock::getMethodPostLoc() const
{
	return (_location.at("method")[1]);
}

void	LocationBlock::setMethodDeleteLoc(std::string method)
{
	_location["method"][2] = method;
}

std::string	LocationBlock::getMethodDeleteLoc() const
{
	return (_location.at("method")[2]);
}

void	LocationBlock::setLocationPath(std::string path)
{
	_location["location_path"][0] = path;
}

std::string	LocationBlock::getLocationPath() const
{
	return (_location.at("location_path")[0]);
}

void	LocationBlock::setLocationModifier(std::string modifier)
{
	_location["location_modifier"][0] = modifier;
}

std::string	LocationBlock::getLocationModifier() const
{
	return (_location.at("location_modifier")[0]);
}

void	LocationBlock::initLocation()
{
	_location["method"].push_back("");
	_location["method"].push_back("");
	_location["method"].push_back("");
	_location["index"].push_back("");
	_location["root"].push_back("");
	_location["autoindex"].push_back("");
	_location["return"].push_back("");
	_location["location_path"].push_back("");
	_location["location_modifier"].push_back("");
}
