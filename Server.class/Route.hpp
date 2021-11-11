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
	void	checkCgi();

	static bool findTarget(const Route & ref, const std::string & target) {
	    return target.find(ref._name) != std::string::npos;
	}

	static int nameDepth(const Route & ref) {
	    int ret = 0;

        if (ref._name != "/")
	        ret = std::count(ref._name.begin(), ref._name.end(), '/');
	    return ret;
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


	std::string getSavePath() const { return _save_path; };
	const std::string getIndexFile() const { return _index_file; };
	const std::string getDirectory() const { return _directory; };
	const std::string getName() const { return _name; };
	const std::string getRedirection() const { return _redirection; };
	const std::string getAllowedMethods();

	bool isMethodInVector(const std::string & methodName);

	bool	isCGI();
	std::string getCGIPath();
	std::string getCGIExt();


};


#endif
