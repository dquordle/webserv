#include "Host.hpp"

std::vector<std::string> Host::addresses;

Host::Host()
{
	_ip = "127.0.0.1";
	_port = 80;
	_portStr = "80";
	_is_default = false;
	_indexOfSocket = -1;
}

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

void Host::setAddress()
{
	_address = _ip + "::" + _portStr;
}

void Host::setDefault(bool isDef)
{
	_is_default = isDef;
}

bool Host::addAddress()
{
	std::vector<std::string>::iterator it = addresses.begin();
	for (; it != addresses.end(); it++)
	{
		if (*it == _address)
			return false;
	}
	addresses.push_back(_address);
	return true;
}

bool Host::isDefault() const
{
	return _is_default;
}

void Host::setSockAddr()
{
	_sockaddrIn.sin_family = AF_INET;
	_sockaddrIn.sin_port = htons(_port);
	_sockaddrIn.sin_addr.s_addr = inet_addr(_ip.c_str());
	_size = sizeof(_sockaddrIn);
}

sockaddr* Host::getSockAddr()
{
	return reinterpret_cast<sockaddr *>(&_sockaddrIn);
}

socklen_t* Host::getSockAddrSize()
{
	return &_size;
}

void Host::setIndex(int index)
{
	_indexOfSocket = index;
}

int Host::getIndex() const
{
	return _indexOfSocket;
}

