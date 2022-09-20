#ifndef REQUEST_CONFIG_HPP
#define REQUEST_CONFIG_HPP

#include "RequestHeader.hpp"
#include "LocationBlock.hpp"
#include "VirtualServerConfig.hpp"

class RequestConfig
{
	private:
		std::string _rootPath;
		bool		_validMethod;
		bool		_isAutoindex;
		bool		_isCgi;
		bool		_isRedirect;
		std::string	_redirectPath;

		bool	isValidMethod(std::string &method, LocationBlock &locBlock, VirtualServerConfig &confBlock);
		void	initRootPath(RequestHeader &request, LocationBlock &locBlock, VirtualServerConfig &confBlock);
		void	isDirectoryPath(LocationBlock &locBlock);
		void	isCgi(std::string &method, LocationBlock &locBlock, RequestHeader &request);
	public:
		RequestConfig();
		~RequestConfig();
		RequestConfig(LocationBlock &locBlock, VirtualServerConfig &confBlock, RequestHeader &request);
		RequestConfig &getRequestConfig();
		RequestConfig &operator=(const RequestConfig &requestConfig);

		const std::string	&getRedirectPath() const;
		const std::string	&getRootPath() const;
		bool				getAutoindex() const;
		bool				getRedirect() const;
		bool				getCgi() const;
		bool				getValidMethod() const;

};

#endif