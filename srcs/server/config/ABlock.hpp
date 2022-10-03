#ifndef ABLOCK_HPP
# define ABLOCK_HPP

#include <string>
#include <map>
#include <vector>

enum boolValue {GET, POST, DELETE, AUTOINDEX};

typedef std::map <std::string, std::vector <std::string> > Serv;

class ABlock
{
	protected:
		Serv	_block;
		int		_port;
		bool	_methodAutoIndex[4];

	public:
		ABlock();
		~ABlock();

		void							setVectorField(const std::vector<std::string> value, std::string field);
		const std::vector<std::string>	&getVectorField(std::string field) const;

		void							setStringField(const std::string value, std::string field);
		const std::string				&getStringField(std::string field) const;

		void							setBoolValue(bool value, int field);
		bool							getBoolValue(int field);
};

#endif
