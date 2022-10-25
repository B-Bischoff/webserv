#include "Autoindex.hpp"

Autoindex::Autoindex(std::string rootPath, std::string fullPath) : _path(fullPath)
{
	_webPath = fullPath.substr(rootPath.length());
	_headBody = "<h1>Index of " + _webPath + "</h1>\n"
			  "<hr/>\n"
			  "<table style=\"width:100%\">\n"
			  "    <tr>\n"
			  "        <th align=\"left\">Name</th>\n"
			  "        <th align=\"left\">Date</th>\n"
			  "        <th align=\"left\">Size</th>\n"
			  "    </tr>\n";
}

Autoindex::~Autoindex()
{

}

void	Autoindex::getDirectoryInfo(std::string &directoryName, struct dirent *ent)
{
	time_t		rawTime;
	struct stat	file;
	struct tm	*timeInfo;
	std::string	stringTime;
	std::string	fullPath = _path + directoryName;

	stat(fullPath.c_str(), &file);
	time(&rawTime);
	timeInfo = localtime(&rawTime);
	stringTime = asctime(timeInfo);
	directoryName = (ent->d_type == DT_REG ? directoryName : directoryName + "/");
	_directoryInfo.push_back(directoryName);
	_directoryInfo.push_back(stringTime);
	std::stringstream ss;
	ss << file.st_size;
	_directoryInfo.push_back((ent->d_type == DT_REG ? ss.str() + "\n" : "-\n"));
}

void	Autoindex::insertTabChar(std::string &directoryName)
{
	int		size = 48;
	int		dirLen = directoryName.size();
	if (dirLen == 8)
		dirLen++;
	int		needed = (size - dirLen) / 8;
	float	check = (size - dirLen) / 8;
	if (needed != check)
		needed++;
	while (needed-- && needed > 0)
	{
		_tmpBody.append("\t");
	}
}

void	Autoindex::addToBody(std::string &directoryName)
{
	_tmpBody = "<tr>\n";
	_tmpBody += "        <td><a href=\"" + directoryName + "\">" + directoryName + "</a></td>\n";
	_tmpBody += "        <td>" + _directoryInfo[1] + "</td>\n";
	_tmpBody += "        <td>" + _directoryInfo[2] + "</td>\n";
	_tmpBody += "    </tr>\n";
}

std::string	Autoindex::buildAutoindex()
{
	DIR				*dir;
	struct dirent	*ent;
	std::string		directoryName;

	if ((dir = opendir (_path.c_str())) != NULL)
	{
  		while ((ent = readdir (dir)) != NULL)
		{
			directoryName = ent->d_name;
			if (directoryName[0] != '.' || (directoryName[0] == '.' && directoryName[1] == '.' && _webPath != "/"))
			{
				getDirectoryInfo(directoryName, ent);
				addToBody(directoryName);
				_headBody.append(_tmpBody);
				_directoryInfo.clear();
			}
  		}
		_headBody+=_endBody;
  		closedir (dir);
	}
	else
		throw(STATUS_404);
	return (_headBody);
}
