#include "ResponseHeader.hpp"

ResponseHeader::ResponseHeader() : protocol("HTTP/1.1 "), 
	content_type("Content-Type: "), content_length("Content-Length: "), closeAfterSend(false) {}

ResponseHeader::~ResponseHeader() {}

std::string	ResponseHeader::get_extension_file(std::string path)
{
	std::string	dst;
	if (path.find(".html\0", 0) != path.npos || path.empty() == true)
		return (dst = "text/html");
	else if (path.find(".png\0", 0) != path.npos)
		return (dst = "image/png");
	else if (path.find(".aac\0", 0) != path.npos)
		return (dst = "audio/aac");
	else if (path.find(".abw\0", 0) != path.npos)
		return (dst = "application/x-abiword");
	else if (path.find(".arc\0", 0) != path.npos)
		return (dst = "application/octet-stream");
	else if (path.find(".avi\0", 0) != path.npos)
		return (dst = "video/x-msvideo");
	else if (path.find(".azw\0", 0) != path.npos)
		return (dst = "application/vnd.amazon.ebook");
	else if (path.find(".bin\0", 0) != path.npos)
		return (dst = "application/octet-stream");
	else if (path.find(".bmp\0", 0) != path.npos)
		return (dst = "image/bmp");
	else if (path.find(".bz\0", 0) != path.npos)
		return (dst = "application/x-bzip");
	else if (path.find(".bz2\0", 0) != path.npos)
		return (dst = "application/x-bzip2");
	else if (path.find(".csh\0", 0) != path.npos)
		return (dst = "application/x-csh");
	else if (path.find(".css\0", 0) != path.npos)
		return (dst = "text/css");
	else if (path.find(".csv\0", 0) != path.npos)
		return (dst = "text/csv");
	else if (path.find(".doc\0", 0) != path.npos)
		return (dst = "application/msword");
	else if (path.find(".docx\0", 0) != path.npos)
		return (dst = "application/vnd.openxmlformats-officedocument.wordprocessingml.document");
	else if (path.find(".eot\0", 0) != path.npos)
		return (dst = "application/vnd.ms-fontobject");
	else if (path.find(".epub\0", 0) != path.npos)
		return (dst = "application/epub+zip");
	else if (path.find(".gif\0", 0) != path.npos)
		return (dst = "image/gif");
	else if (path.find(".ico\0", 0) != path.npos)
		return (dst = "image/x-icon");
	else if (path.find(".ics\0", 0) != path.npos)
		return (dst = "text/calendar");
	else if (path.find(".jar\0", 0) != path.npos)
		return (dst = "application/java-archive");
	else if (path.find(".jpeg\0", 0) != path.npos)
		return (dst = "image/jpeg");
	else if (path.find(".js\0", 0) != path.npos)
		return (dst = "application/javascript");
	else if (path.find(".json\0", 0) != path.npos)
		return (dst = "application/json");
	else if (path.find(".midi\0", 0) != path.npos)
		return (dst = "audio/midi");
	else if (path.find(".mpeg\0", 0) != path.npos)
		return (dst = "video/mpeg");
	else if (path.find(".mpkg\0", 0) != path.npos)
		return (dst = "application/vnd.apple.installer+xml");
	else if (path.find(".odp\0", 0) != path.npos)
		return (dst = "application/vnd.oasis.opendocument.presentation");
	else if (path.find(".ods\0", 0) != path.npos)
		return (dst = "application/vnd.oasis.opendocument.spreadsheet");
	else if (path.find(".odt\0", 0) != path.npos)
		return (dst = "application/vnd.oasis.opendocument.text");
	else if (path.find(".oga\0", 0) != path.npos)
		return (dst = "audio/ogg");
	else if (path.find(".ogv\0", 0) != path.npos)
		return (dst = "video/ogg");
	else if (path.find(".ogx\0", 0) != path.npos)
		return (dst = "application/ogg");
	else if (path.find(".otf\0", 0) != path.npos)
		return (dst = "font/otf");
	else if (path.find(".pdf\0", 0) != path.npos)
		return (dst = "application/pdf");
	else if (path.find(".ppt\0", 0) != path.npos)
		return (dst = "application/vnd.ms-powerpoint");
	else if (path.find(".pptx\0", 0) != path.npos)
		return (dst = "application/vnd.openxmlformats-officedocument.presentationml.presentation");
	else if (path.find(".rar\0", 0) != path.npos)
		return (dst = "application/x-rar-compressed");
	else if (path.find(".rtf\0", 0) != path.npos)
		return (dst = "application/rtf");
	else if (path.find(".sh\0", 0) != path.npos)
		return (dst = "application/x-sh");
	else if (path.find(".svg\0", 0) != path.npos)
		return (dst = "image/svg+xml");
	else if (path.find(".swf\0", 0) != path.npos)
		return (dst = "application/x-shockwave-flash");
	else if (path.find(".tar\0", 0) != path.npos)
		return (dst = "application/x-tar");
	else if (path.find(".tiff\0", 0) != path.npos)
		return (dst = "image/tiff");
	else if (path.find(".ts\0", 0) != path.npos)
		return (dst = "application/typescript");
	else if (path.find(".ttf\0", 0) != path.npos)
		return (dst = "font/ttf");
	else if (path.find(".vsd\0", 0) != path.npos)
		return (dst = "application/vnd.visio");
	else if (path.find(".wav\0", 0) != path.npos)
		return (dst = "audio/x-wav");
	else if (path.find(".weba\0", 0) != path.npos)
		return (dst = "audio/webm");
	else if (path.find(".webm\0", 0) != path.npos)
		return (dst = "video/webm");
	else if (path.find(".webp\0", 0) != path.npos)
		return (dst = "image/webp");
	else if (path.find(".woff\0", 0) != path.npos)
		return (dst = "font/woff");
	else if (path.find(".woff2\0", 0) != path.npos)
		return (dst = "font/woff2");
	else if (path.find(".xhtml\0", 0) != path.npos)
		return (dst = "application/xhtml+xml");
	else if (path.find(".xls\0", 0) != path.npos)
		return (dst = "application/vnd.ms-excel");
	else if (path.find(".xlsx\0", 0) != path.npos)
		return (dst = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
	else if (path.find(".xml\0", 0) != path.npos)
		return (dst = "application/xml");
	else if (path.find(".xul\0", 0) != path.npos)
		return (dst = "application/vnd.mozilla.xul+xml");
	else if (path.find(".zip\0", 0) != path.npos)
		return (dst = "application/zip");
	else if (path.find(".3gp\0", 0) != path.npos)
		return (dst = "video/3gpp");
	else if (path.find(".3g2\0", 0) != path.npos)
		return (dst = "video/3gpp2");
	else if (path.find(".7z\0", 0) != path.npos)
		return (dst = "application/x-7z-compressed");
	else if (path.find(".cpp\0", 0) != path.npos)
		return (dst = "text/x-c");
	else if (path.find(".mp4\0", 0) != path.npos)
		return (dst = "video/mp4");
	else if (path.find(".php\0", 0) != path.npos)
		return (dst = "text/html");
	else if (path.find(".txt\0", 0) != path.npos)
		return (dst = "text/plain");
	else if (path.find(".jpg\0", 0) != path.npos)
		return (dst = "image/jpeg");
	else
		throw(STATUS_415);
}

void	ResponseHeader::build_response(Method &method)
{
	std::ostringstream	convert;
	std::string			ext;
	if (method.getConf().getAutoindex() == true || method.getConf().getRedirect() == true)
		ext = get_extension_file(".html");
	else
		ext = get_extension_file(method.getPath());
	convert << method.getSize();

	response_header = protocol + method.getStatus() + "\r\n" + content_type + ext +"; charset=UTF-8" + "\r\n";
	if (method.getConf().getRedirect() == true) 
		response_header += "Location: " + method.getConf().getRedirectPath() + "\r\n";
	response_header += content_length + convert.str() + "\r\n\r\n" + method.getBody();
	std::cout << ext << std::endl;
}
