#include "Post.hpp"

Post::Post()
{
	
}

Post::~Post()
{

}


Method	Post::exec(RequestConfig &config, const std::vector<BodyData> &bodyData, std::string &cgiResult)
{
	if (config.getUpload() != "")
	{
		uploadFiles(config, bodyData);
		setResponseValue(_body, STATUS_204, _requestConfig.getRootPath());
	}
	if (config.getCgi() == true)
		setResponseValue(cgiResult, STATUS_200, _requestConfig.getRootPath());
	else
		setResponseValue("", STATUS_204, _requestConfig.getRootPath());
	return (*this);
}

void Post::uploadFiles(RequestConfig &config, const std::vector<BodyData> &bodyData)
{
	for (_filePos = 0; _filePos < static_cast<int>(bodyData.size() - 1); _filePos++)
	{
		std::string fileName = getFileName(bodyData);
		if (fileName == "")
			throw (STATUS_400);
		createFile(fileName, config.getUpload(), bodyData[_filePos].content);
	}
}

std::string	Post::getFileName(const std::vector<BodyData> &bodyData)
{
	std::map<std::string, std::string>::const_iterator	it;
	try
	{
		it = bodyData[_filePos].metadata.find("filename");
		if (it != bodyData[_filePos].metadata.end())
			return (it->second);
		return ("");
	}
	catch(const std::exception& e)
	{

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

