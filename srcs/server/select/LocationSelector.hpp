#ifndef LOCATION_SELECTOR_HPP
# define LOCATION_SELECTOR_HPP

#include "webserv.hpp"

class LocationSelector
{
	private:
		static int				endingWildcard(const std::string &path, std::string locationPath);
		static int				startingWildcard(const std::string &path, std::string locationPath);

	public:
		static LocationBlock	selectLocationBlock(const std::string &path, std::vector<LocationBlock> &serverBlock);
};

#endif
