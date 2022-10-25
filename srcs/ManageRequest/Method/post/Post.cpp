#include "Post.hpp"

Post::Post()
{
	
}

Post::~Post()
{

}


Method	Post::exec(RequestConfig &config, const std::vector<BodyData> &bodyData, std::string &cgiResult)
{
	if (config.getUpload() != "" || config.getCgi() == true)
	{
		if (config.getUpload() != "")
		{
			std::string	fileName = getFileName(bodyData);
			if (fileName == "")
				throw(STATUS_NO_FILENAME);
			createFile(fileName, config.getUpload(), bodyData[_filePos].content);
			setResponseValue(_body, STATUS_204, _requestConfig.getRootPath());
		}
		else if (config.getCgi() == true)
			setResponseValue(cgiResult, STATUS_200, _requestConfig.getRootPath());
	}
	else
		setResponseValue("", STATUS_204, _requestConfig.getRootPath());
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

void	Post::createFile(const std::string &fileName, const std::string &path, const std::string &body)
{
	std::ofstream			file;

	std::string outfile = path + "/" + fileName;
	file.open(outfile.c_str(), std::ofstream::out | std::ios_base::binary);
	if (file.is_open() == false)
		throw(STATUS_403);
	file << body;
	file.close();
}
