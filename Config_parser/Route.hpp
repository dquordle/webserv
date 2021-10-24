#ifndef WEB_SERVER_ROUTE_HPP
#define WEB_SERVER_ROUTE_HPP

//#include <iostream>
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

	static size_t findTarget(const Route & ref, const std::string & target) {
	    std::string name = ref._name;
	    name.erase(0);
	    name.erase(name.length());
	    return target.find(name, 0);
	}

	static size_t nameDepth(const Route & ref) {
	    return 	std::count(ref._name.begin(), ref._name.end(), '/');
	}

	Route(const Route & rhs) : _name(rhs._name), _redirection(rhs._redirection), _directory(rhs._directory), _autoindexOn(rhs._autoindexOn),
	_index_file(rhs._index_file), _cgi_path(rhs._cgi_path), _cgi_ext(rhs._cgi_ext), _save_path(rhs._save_path)
	{
        _allowed_methods = rhs._allowed_methods;
	}

	Route & operator=(const Route & rhs) {
	    if (&rhs == this)
	        return *this;
	    _name = rhs._name;
	    _redirection = rhs._redirection;
	    _directory = rhs._directory;
	    _autoindexOn = rhs._autoindexOn;
	    _index_file = rhs._index_file;
	    _cgi_path = rhs._cgi_path;
	    _cgi_ext = rhs._cgi_ext;
	    _save_path = rhs._save_path;
	    _allowed_methods = rhs._allowed_methods;
	    return *this;
	}

	bool isAutoindexOn() { return _autoindexOn; }
};


#endif
