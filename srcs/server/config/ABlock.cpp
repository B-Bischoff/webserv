#include "ABlock.hpp"

ABlock::ABlock() : _port(8080)
{
	_methodAutoIndex[0] = 0;
	_methodAutoIndex[1] = 0;
	_methodAutoIndex[2] = 0;
	_methodAutoIndex[3] = 0;
	_block["index"].push_back("");
	_block["root"].push_back("");
	_block["return"].push_back("");
	_block["location_modifier"].push_back("");
	_block["location_path"].push_back("");
	_block["cgi_pass"].push_back("");
	_block["server_name"].push_back("default");
	_block["access_log"].push_back("");
	_block["error_log"].push_back("");
	_block["ip"].push_back("0.0.0.0");
	_block["error_path"].push_back("");
	_block["upload"].push_back("");
}

ABlock::~ABlock()
{

}

void	ABlock::setStringField(const std::string value, std::string field)
{
	_block[field][0] = value;
}

const std::string	&ABlock::getStringField(std::string field) const
{
	return (_block.at(field)[0]);
}

void	ABlock::setVectorField(const std::vector<std::string> value, std::string field)
{
	_block[field] = value;
}

const std::vector<std::string>	&ABlock::getVectorField(std::string field) const
{
	return (_block.at(field));
}

void	ABlock::setBoolValue(bool value, int field)
{
	_methodAutoIndex[field] = value;
}

bool	ABlock::getBoolValue(int field)
{
	return (_methodAutoIndex[field]);
}
