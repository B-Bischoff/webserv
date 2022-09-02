#include "Server.hpp"
#include "VirtualServerConfig.hpp"

void addServerConfig(std::vector<VirtualServerConfig>& v, std::string name, std::string ip, std::string port)
{
	VirtualServerConfig conf;

	std::vector<std::string> names;
	names.push_back(name);

	conf.setServerName(names);
	conf.setListenIp(ip);
	conf.setListenPort(port);

	v.push_back(conf);
}

int main(void)
{
	//Server Server;

	std::vector<VirtualServerConfig> configList;
	addServerConfig(configList, "localhost", "127.0.0.1", "8080");
	addServerConfig(configList, "random", "0.0.0.0", "8081");

	Server server(configList);

	return 0;
}