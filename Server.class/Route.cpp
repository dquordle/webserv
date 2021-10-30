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
	_redirection = redirection;
}

void Route::setDirectory(const std::string &directory)
{
	_directory = directory;
}

void Route::setAutoindex(const std::string &autoindex)
{
	if (autoindex == "on")
		_autoindexOn = true;
	else if (autoindex != "off")
		exit(1);
}

void Route::setIndexFile(const std::string &indexFile)
{
	_index_file = indexFile;
}

void Route::setCgiPath(const std::string &cgiPath)
{
	_cgi_path = cgiPath;
}

void Route::setCgiExt(const std::string &cgiExt)
{
	_cgi_ext = cgiExt;
}

void Route::setSavePath(const std::string &savePath)
{
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
			exit(1);
		if (Methods::isAllowed(methStr))
			_allowed_methods.push_back(method);
		prev_pos = pos + 1;
		pos = methods.find(',', prev_pos);
	}
	std::string methStr = methods.substr(prev_pos);
	Method method = Methods::getMethodStoE(methStr);
	if (method == NOT_IMPLEMENTED)
		exit(1);
	if (Methods::isAllowed(methStr))
		_allowed_methods.push_back(method);
}

