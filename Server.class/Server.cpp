#include "Server.hpp"

Server::Server()
{
	_ip = DEFAULT_IP;
	_portStr = DEFAULT_PORT_STR;
	sscanf(_portStr.c_str(), "%d", &_port);
	_is_default = false;
}

void Server::setIP(const std::string& ip)
{
	if (ip.empty())
		Debug::FatalError("Configuration file error");
	_ip = ip;
}

void Server::setPort(const std::string& port)
{
	size_t pos = port.find_first_not_of("0123456789");
	if (pos != std::string::npos || port.empty())
		Debug::FatalError("Configuration file error");
	_portStr = port;
	sscanf(port.c_str(), "%d", &_port);
}

void Server::setServerName(const std::string & name)
{
	if (name.empty())
		Debug::FatalError("Configuration file error");
	size_t prev_pos = 0;
	size_t pos = name.find(',');
	while (pos != std::string::npos)
	{
		size_t count = pos - prev_pos;
		std::string serverName = name.substr(prev_pos, count);
		_server_names.push_back(serverName);
		prev_pos = pos + 1;
		pos = name.find(',', prev_pos);
	}
	std::string serverName = name.substr(prev_pos, pos);
	_server_names.push_back(serverName);
}

void Server::addError(const std::string & error)
{
	if (error.empty())
		Debug::FatalError("Configuration file error");
	size_t pagePos = error.find_first_not_of("0123456789");
	if (pagePos == 0 || pagePos % 3 != 0)
		Debug::FatalError("Configuration file error");
	std::string errorPage = error.substr(pagePos);
	if (errorPage.empty())
		Debug::FatalError("Configuration file error");
	size_t pos = 0;
	while (pos < pagePos)
	{
		int code;
		std::string codeStr = error.substr(pos, 3);
		sscanf(codeStr.c_str(), "%d", &code);
		_errors[code] = errorPage;
		pos += 3;
	}
}

void Server::setMaxBodySize(const std::string & sizeStr)
{
	if (sizeStr.empty())
		Debug::FatalError("Configuration file error");
	int size;
	sscanf(sizeStr.c_str(), "%d", &size);
	_max_body_size = size;

}

void Server::addRoute(Route &route)
{
	_routes.push_back(route);
}

void Server::setDefault(bool isDef)
{
	_is_default = isDef;
}

std::string Server::getIp()
{
	return _ip;
}

std::string Server::getPortStr()
{
	return _portStr;
}

std::string Server::isNonDefaultErrorPage(int statusCode) const {
	std::map<int, std::string>::const_iterator it;

	it = _errors.find(statusCode);
	if (it == _errors.end())
		return std::string();
	return it->second;
}

Route * Server::chooseRoute(const std::string & target) {
	std::vector<Route>::iterator it = _routes.begin();
	std::vector<Route>::iterator ite = _routes.end();

//	for(; it != ite; it++)
//	{
//		if (it->isCGI())
//		{
//			std::string cgiExt = it->getCGIExt();
//			if (target.find(cgiExt) == target.length() - cgiExt.length())
//				return new Route(*it);
//		}
//	}
//	it = _routes.begin();
	int maxdepth = -1;
	int currentdepth = -1;
	Route ref = *it;

	for (; it != ite; ++it) {
		if (Route::findTarget((*it), target)) {
			currentdepth = Route::nameDepth(*it);
			if (currentdepth > maxdepth)
			{
				maxdepth = currentdepth;
				ref = *it;
			}
		}
	}
	if (maxdepth == -1)
		return nullptr;
	return new Route(ref);
}

Route * Server::chooseRoutee(const std::string & target) {
	std::vector<Route>::iterator it = _routes.begin();
	std::vector<Route>::iterator ite = _routes.end();

	for(; it != ite; it++)
	{
		if (it->isCGI())
		{
			std::string cgiExt = it->getCGIExt();
			if (target.find(cgiExt) == target.length() - cgiExt.length())
				return new Route(*it);
		}
	}
	it = _routes.begin();
	int maxdepth = -1;
	int currentdepth = -1;
	Route ref = *it;

	for (; it != ite; ++it) {
		if (Route::findTarget((*it), target)) {
			currentdepth = Route::nameDepth(*it);
			if (currentdepth > maxdepth)
			{
				maxdepth = currentdepth;
				ref = *it;
			}
		}
	}
	if (maxdepth == -1)
		return nullptr;
	return new Route(ref);
}

std::vector<std::string> Server::getNames()
{
	return _server_names;
}
