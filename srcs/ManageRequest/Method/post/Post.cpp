#include "Post.hpp"

Post::Post()
{
	
}

Post::~Post()
{

}


Method	Post::exec(RequestConfig &config, const std::string &body, unsigned int size)
{
	(void)config;
	(void)body;
	if (config.getUpload() != "")
	{
		std::string	fileName = getFileName(body);
		//Create a file with body content and fileName name
		createFile(body, fileName, config.getUpload(), size);
		_status = STATUS_200;
		_body += "<a href=\"\">Your file has been upload</a>";
		return (*this);
	}
	if (config.getCgi() == true)
	{

	}
	return (*this);
}

std::string	Post::getFileName(const std::string &body)
{
	std::string	fileName;
	std::string::size_type	pos = body.find("filename=\"");
	if (pos != std::string::npos)
	{
		pos += 10;
		while (body[pos] != '\"')
		{
			fileName += body[pos];
			pos++;
		}
	}
	return (fileName);
}

//Create a file with body content and fileName name to upload directory
void	Post::createFile(const std::string &body, const std::string &fileName, const std::string &path, unsigned int size)
{
	std::string::size_type	pos = body.find("\r\n\r\n");
	std::ofstream			file;
	std::string				tmp;

	for (unsigned int i = 0; i < size; i++)
	{
		tmp += body[pos + 4 + i];
	}
	if (pos != std::string::npos)
	{
		pos += 4;
		std::string outfile = path + fileName;
		file.open(outfile.c_str(), std::ofstream::out | std::ofstream::app);
		if (file.is_open() == false)
		{
			perror("open file: ");
			file.close();
		}
		file << tmp;
		file.close();
	}
}
