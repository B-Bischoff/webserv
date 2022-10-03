#ifndef LOCATION_SELECTOR_HPP
# define LOCATION_SELECTOR_HPP

#include "webserv.hpp"

class LocationSelector
{
	private:
		std::vector<std::string>	_locationPaths;
	public:
		LocationBlock	selectLocationBlock(const std::string &path, std::vector<LocationBlock> &serverBlock);
		int				endingWildcard(const std::string &path, std::string locationPath);
		int				startingWildcard(const std::string &path, std::string locationPath);
};

#endif
