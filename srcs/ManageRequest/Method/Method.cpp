#include "Method.hpp"

Method::Method(RequestHeader request)
{
	(void)request;
}

Method::Method()
{

}

Method::~Method()
{

}

int	Method::getSize() const
{
	return (this->_size);
}

std::string	Method::getBody() const
{
	return (this->_body);
}

std::string	Method::getPath() const
{
	return (this->_path);
}

std::string	Method::getStatus() const
{
	return (this->_status);
}

// std::string	getValueStatus(std::string status, int statusValue)
// {
// 100	Continue
// 101	Switching Protocols
// 102	Processing
// 103	Early Hints
// 200	OK
// 201	Created
// 202	Accepted
// 203	Non-Authoritative Information
// 204	No Content
// 205	Reset Content
// 206	Partial Content
// 207	Multi-Status
// 208	Already Reported
// 210	Content Different
// 226	IM Used
// 300	Multiple Choices
// 301	Moved Permanently
// 302	Found
// 303	See Other
// 304	Not Modified
// 305	Use Proxy
// 306	Switch Proxy
// 307	Temporary Redirect
// 308	Permanent Redirect
// 310	Too many Redirects
// 400	Bad Request
// 401	Unauthorized
// 402	Payment Required
// 403	Forbidden
// 404	Not Found
// 405	Method Not Allowed
// 406	Not Acceptable
// 407	Proxy Authentication Required
// 408	Request Time-out
// 409	Conflict
// 410	Gone
// 411	Length Required
// 412	Precondition Failed
// 413	Request Entity Too Large
// 414	Request-URI Too Long
// 415	Unsupported Media Type
// 416	Requested range unsatisfiable
// 417	Expectation failed
// 418	I’m a teapot
// 421	Bad mapping / Misdirected Request
// 422	Unprocessable entity
// 423	Locked
// 424	Method failure
// 425	Too Early
// 426	Upgrade Required
// 428	Precondition Required
// 429	Too Many Requests
// 431	Request Header Fields Too Large
// 449	Retry With
// 450	Blocked by Windows Parental Controls
// 451	Unavailable For Legal Reasons
// 456	Unrecoverable Error
// 444	No Response
// 495	SSL Certificate Error
// 496	SSL Certificate Required
// 497	HTTP Request Sent to HTTPS Port
// 498	Token expired/invalid
// 499	Client Closed Request
// 500	Internal Server Error
// 501	Not Implemented
// 502	Bad Gateway
// 503	Service Unavailable
// 504	Gateway Time-out
// 505	HTTP Version not supported
// 506	Variant Also Negotiates
// 507	Insufficient storage
// 508	Loop detected
// 509	Bandwidth Limit Exceeded
// 510	Not extended
// 511	Network authentication required
// 520	Unknown Error
// 521	Web Server Is Down
// 522	Connection Timed Out
// 523	Origin Is Unreachable
// 524	A Timeout Occurred
// 525	SSL Handshake Failed
// 526	Invalid SSL Certificate
// 527	Railgun Error

// }


// void	Method::updateStatus(Method method, int statusValue)
// {

// }