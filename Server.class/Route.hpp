#ifndef WEB_SERVER_ROUTE_HPP
#define WEB_SERVER_ROUTE_HPP

#include <vector>
#include "../IHTTPMessage.interface/Methods.hpp"

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
	std::vector<Method>		_allowed_methods;

public:
	explicit Route(const std::string & name);
	~Route();

	void setRedirection(const std::string& redirection);
	void setDirectory(const std::string& directory);
	void setAutoindex(const std::string& autoindex);
	void setIndexFile(const std::string& indexFile);
	void setCgiPath(const std::string& cgiPath);
	void setCgiExt(const std::string& cgiExt);
	void setSavePath(const std::string& savePath);
	void setMethods(const std::string& methods);
};


#endif
