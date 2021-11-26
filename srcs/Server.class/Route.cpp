#include "Route.hpp"

Route::Route(const std::string & name) : _name(name)
{
	_allowed_methods.push_back(GET);
	_allowed_methods.push_back(POST);
	_allowed_methods.push_back(DELETE);
	_allowed_methods.push_back(HEAD);
	_allowed_methods.push_back(PUT);
	_autoindexOn = false;
	_max_body_size = 0;
}

Route::~Route() {}

void Route::setRedirection(const std::string &redirection)
{
	if (redirection.empty())
		Debug::FatalError("Configuration file error");
	_redirection = redirection;
}

void Route::setDirectory(const std::string &directory)
{
	if (directory.empty())
		Debug::FatalError("Configuration file error");
	_directory = directory;
}

void Route::setAutoindex(const std::string &autoindex)
{
	if (autoindex == "on")
		_autoindexOn = true;
	else if (autoindex != "off")
		Debug::FatalError("Configuration file error");
}

void Route::setIndexFile(const std::string &indexFile)
{
	if (indexFile.empty())
		Debug::FatalError("Configuration file error");
	_index_file = indexFile;
}

void Route::setCgiPath(const std::string &cgiPath)
{
	if (cgiPath.empty())
		Debug::FatalError("Configuration file error");
	_cgi_path = cgiPath;
}

void Route::setCgiExt(const std::string &cgiExt)
{
	if (cgiExt.empty())
		Debug::FatalError("Configuration file error");
	_cgi_ext = cgiExt;
}

void Route::setSavePath(const std::string &savePath)
{
	if (savePath.empty())
		Debug::FatalError("Configuration file error");
	_save_path = savePath;
}

void Route::setMethods(const std::string &methods)
{
	_allowed_methods.clear();

	size_t prev_pos = 0;
	size_t pos = methods.find(',');
	while (pos != std::string::npos)
	{
		size_t count = pos - prev_pos;
		std::string methStr = methods.substr(prev_pos, count);
		Method method = Methods::getMethodStoE(methStr);
		if (method == NOT_IMPLEMENTED)
			Debug::FatalError("Configuration file error");
		if (Methods::isAllowed(methStr))
			_allowed_methods.push_back(method);
		prev_pos = pos + 1;
		pos = methods.find(',', prev_pos);
	}
	std::string methStr = methods.substr(prev_pos);
	Method method = Methods::getMethodStoE(methStr);
	if (method == NOT_IMPLEMENTED)
		Debug::FatalError("Configuration file error");
	if (Methods::isAllowed(methStr))
		_allowed_methods.push_back(method);
	if (_allowed_methods.empty())
		Debug::FatalError("Configuration file error");
}

bool Route::isMethodInVector(const std::string & methodName)  {
    Method method = Methods::getMethodStoE(methodName);

    std::vector<Method>::iterator it = _allowed_methods.begin();
    std::vector<Method>::iterator ite = _allowed_methods.end();

    if (find(it, ite, method) != ite)
        return true;
    return false;
}

const std::string Route::getAllowedMethods() {
    std::vector<Method>::iterator it = _allowed_methods.begin();
    std::string ret;

    for (; it != _allowed_methods.end(); it++) {
        ret.append(Methods::getMethodEtoS(*it));
        ret.append(", ");
    }
    return ret;
}

bool Route::isCGI()
{
	if (_cgi_ext.empty() || _cgi_path.empty())
		return false;
	return true;
}

void Route::checkCgi()
{
	if (_cgi_path.empty() ^ _cgi_ext.empty())
		Debug::FatalError("Configuration file error");
}

std::string Route::getCGIPath()
{
	return _cgi_path;
}

std::string Route::getCGIExt()
{
	return _cgi_ext;
}

void Route::setMaxBodySize(const std::string &sizeStr)
{
	if (sizeStr.empty())
		Debug::FatalError("Configuration file error");
	int size;
	sscanf(sizeStr.c_str(), "%d", &size);
	_max_body_size = size;
}

size_t Route::getMaxBodySize()
{
	return _max_body_size;
}

int Route::nameDepth(const Route & ref) {
    int ret = 0;

    if (ref._name != "/")
        ret = std::count(ref._name.begin(), ref._name.end(), '/');
    return ret;
}

Route::Route(const Route & rhs) : _name(rhs._name), _redirection(rhs._redirection), _directory(rhs._directory), _autoindexOn(rhs._autoindexOn),
                           _index_file(rhs._index_file), _cgi_path(rhs._cgi_path), _cgi_ext(rhs._cgi_ext), _save_path(rhs._save_path), _max_body_size(rhs._max_body_size)
{
    _allowed_methods = rhs._allowed_methods;
}

bool Route::findTarget(const Route & ref, const std::string & target) {
    return target.find(ref._name) != std::string::npos;
}

Route & Route::operator=(const Route & rhs) {
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

bool Route::isAutoindexOn() { return _autoindexOn; }

