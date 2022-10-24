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
		// Need to create a function to create every file in bodyData vector
		//createFile("SecondFile.mp4", config.getUpload(), bodyData[1].content);

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
	{
		errno = 0;
		if (errno == 13)
			throw(STATUS_403);
		throw(STATUS_409);
	}
	file << body;
	file.close();
}
