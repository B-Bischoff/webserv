#ifndef ABLOCK_HPP
# define ABLOCK_HPP

#include <string>
#include <map>
#include <vector>

typedef std::map <std::string, std::vector <std::string> > Serv;

class ABlock
{
	protected:
		Serv	_block;
		bool	_autoIndex;
		bool	_get;
		bool	_post;
		bool	_delete;

	public:
		ABlock();
		~ABlock();
		
		void						setMethodGet(bool value);
		bool						getMethodGet() const;

		void						setMethodPost(bool value);
		bool						getMethodPost() const;

		void						setMethodDelete(bool value);
		bool						getMethodDelete() const;

		void						setAutoIndex(bool value);
		bool						getAutoIndex(void) const;

		void						setRoot(std::string path);
		std::string					getRoot(void) const;

		void						setIndex(std::string path);
		std::string					getIndex(void) const;

		void						setReturn(std::vector<std::string> value);
		std::vector<std::string>	getReturn(void) const;
};

#endif