#ifndef POST_HPP
# define POST_HPP

#include "Method.hpp"
#include <algorithm>

class Post : public Method
{
	private:
		int	_filePos;	

		std::string	getFileName(const std::vector<BodyData> &bodyData);
		void		createFile(const std::string &fileName, const std::string &path, const std::string &body);
	public:
		Post();
		~Post();

		Method	exec(RequestConfig &config, const std::vector<BodyData> &bodyData);
};

#endif
