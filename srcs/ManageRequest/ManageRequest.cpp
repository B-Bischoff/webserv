#include "ManageRequest.hpp"

ManageRequest::ManageRequest(Client &client) : _client(client)
{

}

ManageRequest::~ManageRequest()
{

}

Method ManageRequest::identify()
{
	RequestConfig requestConfig(_client.locationBlock, _client.virtualServer, _client.request);
	Method index(requestConfig);
	std::string	cgiResult;

	if (requestConfig.getValidMethod() == false)
		throw(STATUS_405);
	else if (requestConfig.getRedirect() == true)
		return (index.redirect(requestConfig));
	else if (requestConfig.getAutoindex() == true)
		return (index.autoindex(_client.locationBlock.getStringField("root") != ""
		? _client.locationBlock.getStringField("root") : _client.virtualServer.getStringField("root"), requestConfig.getRootPath()));
	else if (requestConfig.getCgi() == true)
		cgiResult = CgiHandler::execCgi(_client);
	if (requestConfig.getMethod() == "GET")
	{
		Get	get;
		return (get.exec(requestConfig, cgiResult));
	}
	else if (requestConfig.getMethod() == "POST")
	{
		Post post;
		return (post.exec(requestConfig, _client.request.getBodydata()));
	}
	else
	{
		Delete	del;
		return (del.exec(requestConfig, cgiResult));
	}
	return (index);
}



