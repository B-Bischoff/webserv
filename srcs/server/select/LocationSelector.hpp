#ifndef LOCATION_SELECTOR_HPP
# define LOCATION_SELECTOR_HPP

#include "webserv.hpp"

class LocationSelector
{
	private:
		std::vector<std::string>	_locationPaths;
	public:
		LocationSelector();
		~LocationSelector();
		LocationBlock	selectLocationBlock(const std::string &path, std::vector<LocationBlock> &serverBlock);
};

#endif