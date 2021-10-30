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
		exit(1);
	_ip = ip;
}

void Server::setPort(const std::string& port)
{
	size_t pos = port.find_first_not_of("0123456789");
	if (pos != std::string::npos || port.empty())
		exit(1);
	_portStr = port;
	sscanf(port.c_str(), "%d", &_port);
}

void Server::setServerName(const std::string & name)
{
	if (name.empty())
		exit(1);
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
		exit(10);
	std::string errorPage = error.substr(3);
	if (errorPage.empty())
		exit(1);
	if (error.find_first_not_of("0123456789") != 3)
		exit(1);
	int code;
	sscanf(error.c_str(), "%d", &code);
	_errors[code] = errorPage;
}

void Server::setMaxBodySize(const std::string & sizeStr)
{
	if (sizeStr.empty())
		exit(1);
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

bool Server::isDefault() const
{
	return _is_default;
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
