#include "LocationSelector.hpp"

LocationSelector::LocationSelector()
{

}

LocationSelector::~LocationSelector()
{

}

LocationBlock	LocationSelector::selectLocationBlock(const std::string &path, std::vector<LocationBlock> &serverBlock)
{
	std::vector<std::string>	validPaths;
	size_t						maxSize = 0;
	int							maxSizeIndex = 0;

	std::cout << "Received path --> " << path << std::endl;
	for (size_t i = 0; i < serverBlock.size(); i++)
	{
		std::cout << serverBlock[i].getLocationPath() << " " << serverBlock[i].getLocationModifier() << std::endl;
		if (serverBlock[i].getLocationPath().find(path) != std::string::npos)
		{
			if (serverBlock[i].getLocationPath() == path && serverBlock[i].getLocationModifier() == "=")
			{
				return (serverBlock[i]);
			}
			_locationPaths.push_back(serverBlock[i].getLocationPath());
			if (serverBlock[i].getLocationPath().size() > maxSize && serverBlock[i].getLocationPath().size() <= path.size())
			{
				maxSize = serverBlock[i].getLocationPath().size();
				maxSizeIndex = i;
			}
		}
	}
	return (serverBlock[maxSizeIndex]);
}