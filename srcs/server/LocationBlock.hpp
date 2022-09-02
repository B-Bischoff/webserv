#ifndef LOCATION_SERVER_BLOCK_HPP
# define LOCATION_SERVER_BLOCK_HPP

#include "VirtualServerConfig.hpp"
typedef std::map <std::string, std::vector <std::string> > Serv;

class LocationBlock
{
	private:
		Serv				_location;

	public:
		LocationBlock();
		~LocationBlock();

		void						setAutoIndexLoc(bool value);
		bool						getAutoIndexLoc() const;

		void						setReturnLoc(std::vector<std::string> value);
		std::vector<std::string>	getReturnLoc() const;

		void						setRootLoc(std::string path);
		std::string					getRootLoc() const;

		void						setIndexLoc(std::string path);
		std::string					getIndexLoc() const;

		void						setMethodGetLoc(std::string method);
		std::string					getMethodGetLoc() const;

		void						setMethodPostLoc(std::string method);
		std::string					getMethodPostLoc() const;

		void						setMethodDeleteLoc(std::string method);
		std::string					getMethodDeleteLoc() const;

		void						setLocationPath(std::string values);
		std::string					getLocationPath() const;

		void						setLocationModifier(std::string values);
		std::string					getLocationModifier() const;

		void						initLocation();
};

#endif