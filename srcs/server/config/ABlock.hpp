#ifndef ABLOCK_HPP
# define ABLOCK_HPP

#include <string>
#include <map>
#include <vector>

#define GET 0
#define POST 1
#define DELETE 2
#define AUTOINDEX 3

typedef std::map <std::string, std::vector <std::string> > Serv;

class ABlock
{
	protected:
		Serv	_block;
		bool	_methodAutoIndex[4];
		bool	_autoIndex;
		bool	_get;
		bool	_post;
		bool	_delete;
		int		_port;

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