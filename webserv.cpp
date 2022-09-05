#include "Server.hpp"
#include "VirtualServerConfig.hpp"

void addServerConfig(std::vector<VirtualServerConfig>& v, std::string name, std::string ip, std::string port)
{
	VirtualServerConfig conf;

	std::vector<std::string> names;
	names.push_back(name);

	conf.setServerName(names);
	conf.setIp(ip);
	conf.setPort(port);

	v.push_back(conf);
}

#include "Parsing.hpp"

int main(void)
{
	std::vector<VirtualServerConfig> configList;
/*
	addServerConfig(configList, "localhost", "127.0.0.1", "8080");
	addServerConfig(configList, "random", "0.0.0.0", "8081");
*/
	Parsing parsing;
	char file[] = "conf/nginx.conf";

	if (parsing.parseConfigFile(file, configList) == -1)
	{
		std::cerr << "Parsing error" << std::endl;
		exit(EXIT_FAILURE);
	}

	Server server(configList);

	return 0;
}