#include "Host.hpp"

//Host::errors::errors(std::vector<int> & codes, std::string & page)
//{
//	this->page = page;
//	this->codes = codes;
//	this->next = NULL;
//}

Host::Host()
{
	_ip = "127.0.0.1";
	_port = 80;
	_portStr = "80";
	_is_default = false;

}

//Host::Host(const Host &host)
//{
//
//}

Host::~Host()
{

}

void Host::setIP(const std::string& ip)
{
	if (ip.empty())
		exit(1);
	_ip = ip;
}

void Host::setPort(const std::string& port)
{
	size_t pos = port.find_first_not_of("0123456789");
	if (pos != std::string::npos || port.empty())
		exit(1);
	_portStr = port;
	sscanf(port.c_str(), "%d", &_port);
}

void Host::setServerName(const std::string & name)
{
	if (name.empty())
		exit(1);
	size_t prev_pos = 0;
	size_t pos = name.find(',');
	while (pos != std::string::npos)
	{
		size_t count = pos - prev_pos;
		std::string serverName = name.substr(prev_pos, count);
		_server_name.push_back(serverName);
		prev_pos = pos + 1;
		pos = name.find(',', prev_pos);
	}
	std::string serverName = name.substr(prev_pos, pos);
	_server_name.push_back(serverName);
}

void Host::addError(const std::string & error)
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

void Host::setMaxBodySize(const std::string & sizeStr)
{
	if (sizeStr.empty())
		exit(1);
	int size;
	sscanf(sizeStr.c_str(), "%d", &size);
	_max_body_size = size;

}

void Host::addRoute(Route &route)
{
	_routes.push_back(route);
}
