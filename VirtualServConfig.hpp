#ifndef VIRTUAL_SERVER_CONFIG_HPP
# define VIRTUAL_SERVER_CONFIG_HPP
#include "conf.hpp"

typedef std::map <std::string, std::vector <std::string> > Serv;

class VirtualServConfig
{
	private:
		Serv			_vServ;												
		unsigned int	_client_max_body_size;
		bool			_autoIndex;

	public:
		VirtualServConfig();
		~VirtualServConfig();

		void						setListenIp(std::string ip);
		std::string					getListenIp(void) const;

		void						setListenPort(std::string ip);
		std::string					getListenPort(void) const;

		void						setServerName(std::vector<std::string> serverName);
		std::vector<std::string>	getServerName(void) const;

		void						setRoot(std::string path);
		std::string					getRoot(void) const;

		void						setIndex(std::string path);
		std::string					getIndex(void) const;

		void						setAccessLog(std::vector<std::string> accessLog);
		std::vector<std::string>	getAccessLog(void) const;

		void						setErrorLog(std::vector<std::string> errorPage);
		std::vector<std::string>	getErrorLog(void) const;

		void						setMethodGet(std::string method);
		std::string					getMethodGet(void) const;

		void						setMethodPost(std::string method);
		std::string					getMethodPost(void) const;

		void						setMethodDelete(std::string method);
		std::string					getMethodDelete(void) const;
};

#endif