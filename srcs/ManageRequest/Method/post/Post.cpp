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
		uploadFiles(config, bodyData);

		_status = STATUS_201;
		_body += "<a href=\"\">Your file has been uploaded</a>";
		_size = _body.size();
		return (*this);
	}
	if (config.getCgi() == true)
	{

	}
	return (*this);
}

void Post::uploadFiles(RequestConfig &config, const std::vector<BodyData> &bodyData)
{
	for (_filePos = 0; _filePos < static_cast<int>(bodyData.size() - 1); _filePos++)
	{
		std::string fileName = getFileName(bodyData);
		if (fileName == "")
			throw (STATUS_NO_FILENAME);
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
	{
		errno = 0;
		if (errno == 13)
			throw(STATUS_403);
		throw(STATUS_409);
	}
	file << body;
	file.close();
}

