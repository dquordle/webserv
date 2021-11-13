#include "Route.hpp"

Route::Route(const std::string & name) : _name(name)
{
	_allowed_methods.push_back(GET);
	_allowed_methods.push_back(POST);
	_allowed_methods.push_back(DELETE);
	_autoindexOn = false;
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

    if (find(_allowed_methods.begin(), _allowed_methods.end(), method) != _allowed_methods.end())
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

const std::string Route::getCgiExt() { return _cgi_ext; }

const std::string Route::getCgiPath() { return _cgi_path; }

