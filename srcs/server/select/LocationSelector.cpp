#include "LocationSelector.hpp"

LocationBlock	LocationSelector::selectLocationBlock(const std::string &path, std::vector<LocationBlock> &serverBlock)
{
	std::vector<std::string>	validPaths;
	size_t						maxSize = 0;
	int							maxSizeIndex = 0;
	std::string					locationPath;

	for (size_t i = 0; i < serverBlock.size(); i++)
	{
		locationPath = serverBlock[i].getStringField("location_path");
		if (path.find(locationPath) != std::string::npos)
		{
			if (locationPath == path && serverBlock[i].getStringField("location_modifier") == "=")
				return (serverBlock[i]);
			_locationPaths.push_back(locationPath);
			if (locationPath.size() > maxSize)
			{
				maxSize = locationPath.size();
				maxSizeIndex = i;
			}
		}
		if (path.find(locationPath) != std::string::npos && locationPath.find('*') != std::string::npos)
		{
			std::cout << "help " << std::endl;
			if (locationPath.find('*') == 0)
			{
				if (endingWildcard(path, locationPath) == 0)
					return (serverBlock[i]);
			}
			else if (locationPath.find('*') == locationPath.length() - 1)
			{
				if (startingWildcard(path, locationPath) == 0)
					return (serverBlock[i]);
			}
		}
	}
	return (serverBlock[maxSizeIndex]);
}

int	LocationSelector::endingWildcard(const std::string &path, std::string locationPath)
{
	return (path.compare(path.length() - (locationPath.length() - 1), locationPath.length(), locationPath.substr(1)));
}

int	LocationSelector::startingWildcard(const std::string &path, std::string locationPath)
{
	return (path.compare(0, locationPath.length() - 1, locationPath.substr(0, locationPath.length() - 1)));
}
