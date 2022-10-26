#ifndef REQUEST_CONFIG_HPP
#define REQUEST_CONFIG_HPP

#include "RequestHeader.hpp"
#include "LocationBlock.hpp"
#include "VirtualServerConfig.hpp"

class RequestConfig
{
	private:
		bool		_validMethod;
		bool		_isAutoindex;
		bool		_isCgi;
		bool		_isRedirect;
		std::string _rootPath;
		std::string	_redirectPath;
		std::string	_upload;
		std::string	_method;

		bool	isValidMethod(std::string &method, LocationBlock &locBlock, VirtualServerConfig &confBlock);
		void	initRootPath(RequestHeader &request, LocationBlock &locBlock, VirtualServerConfig &confBlock);
		void	isDirectoryPath(LocationBlock &locBlock, RequestHeader &request);
		void	isCgi(LocationBlock &locBlock);
	public:
		RequestConfig();
		~RequestConfig();
		RequestConfig(LocationBlock &locBlock, VirtualServerConfig &confBlock, RequestHeader &request);

		const std::string	&getRedirectPath() const;
		const std::string	&getRootPath() const;
		const std::string	&getMethod() const;
		const std::string	&getUpload() const;
		bool				getAutoindex() const;
		bool				getRedirect() const;
		bool				getCgi() const;
		bool				getValidMethod() const;

};

#endif
