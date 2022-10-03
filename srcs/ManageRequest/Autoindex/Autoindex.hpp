#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iomanip>
#include <time.h> 
#include <vector>
#include <string>
#include <sstream>
#include "Method.hpp"

class Autoindex
{
	private:
		std::string					_headBody;
		std::string					_endBody;
		std::string					_path;
		std::vector<std::string>	_directoryInfo;
		std::string					_tmpBody;

		void	getDirectoryInfo(std::string &directoryName, struct dirent *ent);
		void	addToBody(std::string &directoryName);
		void	insertTabChar(std::string &directoryName);

	public:
		Autoindex(std::string path);
		~Autoindex();
		std::string	buildAutoindex();
};

#endif
