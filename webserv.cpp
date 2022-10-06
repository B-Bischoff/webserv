#include "Server.hpp"
#include "VirtualServerConfig.hpp"
#include "Parsing.hpp"

int main(void)
{
	std::vector<VirtualServerConfig> configList;

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
