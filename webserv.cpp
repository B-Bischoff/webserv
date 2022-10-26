#include "Server.hpp"
#include "VirtualServerConfig.hpp"
#include "Parsing.hpp"

int main(int argc, char **argv)
{
	(void)argc;
	std::vector<VirtualServerConfig> configList;

	Parsing parsing;
	if (argc != 2)
	{
		std::cerr << "Usage: ./webserv <conf_file>" << std::endl;
		return (0);
	}
	if (parsing.parseConfigFile(argv[1], configList) == -1)
	{
		std::cerr << "Parsing error" << std::endl;
		exit(EXIT_FAILURE);
	}

	signal(SIGPIPE, SIG_IGN); // return error when client break the socket while the server send a response
	Server server(configList);

	return 0;
}
