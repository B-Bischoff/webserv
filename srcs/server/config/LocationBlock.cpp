#include "LocationBlock.hpp"

LocationBlock::LocationBlock()
{
	_block["location_path"].push_back("");
	_block["location_modifier"].push_back("");
}

LocationBlock::~LocationBlock()
{
	
}

void	LocationBlock::setLocationPath(std::string path)
{
	_block["location_path"][0] = path;
}

std::string	LocationBlock::getLocationPath() const
{
	return (_block.at("location_path")[0]);
}

void	LocationBlock::setLocationModifier(std::string modifier)
{
	_block["location_modifier"][0] = modifier;
}

std::string	LocationBlock::getLocationModifier() const
{
	return (_block.at("location_modifier")[0]);
}

