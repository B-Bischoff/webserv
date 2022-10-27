#include "LocationSelector.hpp"
bool compareWildCard(const std::string& reference, const std::string& target);
LocationBlock	LocationSelector::selectLocationBlock(const std::string &path, std::vector<LocationBlock> &serverBlock)
{
	std::vector<std::string>	validPaths;
	std::vector<std::string>	locationPaths;
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
			locationPaths.push_back(locationPath);
			if (locationPath.size() > maxSize)
			{
				maxSize = locationPath.size();
				maxSizeIndex = i;
			}
		}
		if (compareWildCard(locationPath, path) == true)
			return (serverBlock[i]);
	}
	return (serverBlock[maxSizeIndex]);
}

bool compareWildCard(const std::string& reference, const std::string& target)
{
	std::vector<std::string> splittedReference;
	if (reference.find_first_not_of('*') == std::string::npos) // reference only contains '*'
		return true;
	int i = reference.find_first_not_of('*');
	while (i < static_cast<int>(reference.length()))
	{
		std::string str;
		int end;
		if (reference.find('*', i) == std::string::npos)
			end = reference.length();
		else
			end = reference.find('*', i);
		str = reference.substr(i, end - i);
		if (str.empty() == false) // consecutive wildcards can create empty str
			splittedReference.push_back(str);
		i = end + 1;
	}

	bool startsWithWildcard = reference[0] == '*';
	bool endsWithWildcard = reference[reference.length() - 1] == '*';
	if (startsWithWildcard == false)
	{
		std::string str = target.substr(0, splittedReference[0].length());
		if (str != splittedReference[0])
			return false;
	}
	if (endsWithWildcard == false)
	{
		const std::string& str = splittedReference[splittedReference.size() - 1];
		if (target.find(str.c_str(), target.length() - str.length(), str.length()) == std::string::npos)
			return false;
	}

	int previousMatch = 0;
	for (int i = 0; i < (int)splittedReference.size(); i++)
	{
		if (target.find(splittedReference[i], previousMatch) != std::string::npos)
			previousMatch = target.find(splittedReference[i], previousMatch) + splittedReference[i].length();
		else
			return false;
	}

	return true;
}