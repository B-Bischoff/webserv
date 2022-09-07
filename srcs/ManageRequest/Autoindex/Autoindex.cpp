#include "Autoindex.hpp"

Autoindex::Autoindex(std::string path) : _path(path)
{
	_headBody = "<html>\n<head><title>Index of ";
	_headBody += path;
	_headBody += "</title><head>\n<body>\n<h1>Index of " + path + "</h1><hr><pre>\n";
	_endBody = "</pre><hr></body>\n</html>";
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

	stat(directoryName.c_str(), &file);
	time(&rawTime);
	timeInfo = localtime(&rawTime);
	stringTime = asctime(timeInfo);
	directoryName = (ent->d_type == DT_REG ? directoryName : directoryName + "/");
	_directoryInfo.push_back(directoryName);
	_directoryInfo.push_back(stringTime);
	_directoryInfo.push_back((ent->d_type == DT_REG ? std::to_string(file.st_size) + "\n" : "-\n"));
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
	while (needed--)
	{
		_tmpBody.append("\t");
	}
}

void	Autoindex::addToBody(std::string &directoryName)
{
	std::string	syntax = "<a href=\"\"></a>";
	_tmpBody = syntax;
	_tmpBody.insert(9, directoryName);
	_tmpBody.insert(11 + directoryName.size(), directoryName);
	insertTabChar(directoryName);
	_tmpBody.append(_directoryInfo[1].erase('\n'));
	_tmpBody.append("\t\t\t\t" + _directoryInfo[2]);
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
			if (directoryName[0] != '.' || (directoryName[0] == '.' && directoryName[1] == '.'))
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
		throw(STATUS_500);
	return (_headBody);
}