#ifndef POST_HPP
# define POST_HPP

#include "Method.hpp"

class Post : public Method
{
	private:
	
		std::string	getFileName(const std::string &body);
		void		createFile(const std::string &body, const std::string &fileName, const std::string &path, unsigned int size);
	public:
		Post();
		~Post();

		Method	exec(RequestConfig &config, const std::string &body, unsigned int size);
};

#endif