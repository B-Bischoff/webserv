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

	for (size_t i = 0; i < serverBlock.size(); i++)
	{
		if (path.find(serverBlock[i].getStringField("location_path")) != std::string::npos)
		{
			if (serverBlock[i].getStringField("location_path") == path && serverBlock[i].getStringField("location_modifier") == "=")
				return (serverBlock[i]);
			_locationPaths.push_back(serverBlock[i].getStringField("location_path"));
			if (serverBlock[i].getStringField("location_path").size() > maxSize && serverBlock[i].getStringField("location_path").size() <= path.size())
			{
				maxSize = serverBlock[i].getStringField("location_path").size();
				maxSizeIndex = i;
			}
		}
	}
	return (serverBlock[maxSizeIndex]);
}