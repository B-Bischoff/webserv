#include "Post.hpp"

Post::Post()
{
	
}

Post::~Post()
{

}


Method	Post::exec(RequestConfig &config, const std::vector<BodyData> &bodyData)
{
	if (config.getUpload() != "")
	{
		std::string	fileName = getFileName(bodyData);
		if (fileName == "")
			throw(STATUS_NO_FILENAME);
		createFile(fileName, config.getUpload(), bodyData[_filePos].content);
		_status = STATUS_201;
		_body += "<a href=\"\">Your file has been upload</a>";
		_body = "";
		return (*this);
	}
	if (config.getCgi() == true)
	{

	}
	return (*this);
}

std::string	Post::getFileName(const std::vector<BodyData> &bodyData)
{
	std::map<std::string, std::string>::const_iterator	it;
	try
	{
		for (size_t i = 0; i < bodyData.size(); i++)
		{
			it = bodyData[i].metadata.find("filename");
			if (it != bodyData[i].metadata.end())
			{
				_filePos = i;
				return (it->second);
			}
		}
		return ("");
	}
	catch(const std::exception& e)
	{
		std::cout << "No filename" << std::endl;
	}
	return ("");
}

//Create a file with body content and fileName name to upload directory
void	Post::createFile(const std::string &fileName, const std::string &path, const std::string &body)
{
	std::ofstream			file;

	std::string outfile = path + "/" + fileName;
		file.open(outfile.c_str(), std::ofstream::out);
	std::cout << outfile << std::endl;
	if (file.is_open() == false)
		throw(STATUS_500);
	file << body;
	file.close();
}
