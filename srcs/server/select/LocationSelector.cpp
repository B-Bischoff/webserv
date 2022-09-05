#include "LocationSelector.hpp"

LocationSelector::LocationSelector()
{

}

LocationSelector::~LocationSelector()
{

}

LocationBlock	LocationSelector::selectLocationBlock(std::string &path, std::vector<LocationBlock> &serverBlock)
{
	std::vector<std::string>	validPaths;
	int							maxSize = 0;
	int							maxSizeIndex = 0;

	for (int i = 0; i < serverBlock.size(); i++)
	{
		if (serverBlock[i].getLocationPath().find(path) != std::string::npos)
		{
			if (serverBlock[i].getLocationPath() == path && serverBlock[i].getLocationModifier() == "=")
				return (serverBlock[i]);
			_locationPaths.push_back(serverBlock[i].getLocationPath());
			if (_locationPaths[i].size() > maxSize && _locationPaths[i].size() <= path.size())
			{
				maxSize = _locationPaths[i].size();
				maxSizeIndex = i;
			}
		}
	}
	return (serverBlock[maxSizeIndex]);
}