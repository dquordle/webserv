#ifndef WEB_SERVER_ROUTE_HPP
#define WEB_SERVER_ROUTE_HPP

#include <vector>
#include "../IHTTPMessage.interface/Methods.hpp"
#include "../Debug.class/Debug.hpp"

class Route
{

private:
	std::string				_name;
	std::string				_redirection;
	std::string				_directory;
	bool					_autoindexOn;
	std::string 			_index_file;
	std::string				_cgi_path;
	std::string				_cgi_ext;
	std::string				_save_path;
	size_t 					_max_body_size;
	std::vector<Method>		_allowed_methods;

public:
	explicit Route(const std::string & name);
	virtual ~Route();

	void setRedirection(const std::string& redirection);
	void setDirectory(const std::string& directory);
	void setAutoindex(const std::string& autoindex);
	void setIndexFile(const std::string& indexFile);
	void setCgiPath(const std::string& cgiPath);
	void setCgiExt(const std::string& cgiExt);
	void setSavePath(const std::string& savePath);
	void setMaxBodySize(const std::string & size);
	void setMethods(const std::string& methods);
	void	checkCgi();

	static bool findTarget(const Route & ref, const std::string & target);

	static int nameDepth(const Route & ref);

	Route(const Route & rhs);

	Route & operator=(const Route & rhs);

	bool isAutoindexOn();


	std::string getSavePath() const { return _save_path; };
	const std::string getIndexFile() const { return _index_file; };
	const std::string getDirectory() const { return _directory; };
	const std::string getName() const { return _name; };
	const std::string getRedirection() const { return _redirection; };
	const std::string getAllowedMethods();
	const std::string getCgiExt();
	const std::string getCgiPath();

	bool isMethodInVector(const std::string & methodName);

	bool	isCGI();
	std::string getCGIPath();
	std::string getCGIExt();
	size_t		getMaxBodySize();


};


#endif
