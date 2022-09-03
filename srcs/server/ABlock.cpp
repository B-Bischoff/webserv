#include "ABlock.hpp"

ABlock::ABlock() : _autoIndex(false), _get(false), _post(false), _delete(false)
{
	_block["index"].push_back("");
	_block["root"].push_back("");
	_block["return"].push_back("");
}

ABlock::~ABlock()
{

}

void	ABlock::setAutoIndex(bool value)
{
	_autoIndex = value;
}

bool	ABlock::getAutoIndex() const
{
	return (_autoIndex);
}

void	ABlock::setMethodGet(bool value)
{
	_get = value;
}

bool	ABlock::getMethodGet() const
{
	return (_get);
}

void	ABlock::setMethodPost(bool value)
{
	_post = value;
}

bool	ABlock::getMethodPost() const
{
	return (_post);
}

void	ABlock::setMethodDelete(bool value)
{
	_delete = value;
}

bool	ABlock::getMethodDelete() const
{
	return (_delete);
}

void	ABlock::setReturn(std::vector<std::string> value)
{
	_block["return"] = value;
}

std::vector<std::string>	ABlock::getReturn() const
{
	return (_block.at("return"));
}

void	ABlock::setRoot(std::string path)
{
	_block["root"][0] = path;
}

std::string	ABlock::getRoot() const
{
	return (_block.at("root")[0]);
}

void	ABlock::setIndex(std::string path)
{
	_block["index"][0] = path;
}

std::string	ABlock::getIndex() const
{
	return (_block.at("index")[0]);
}