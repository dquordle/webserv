#include "ServersFamily.hpp"

ServersFamily::ServersFamily(const std::string& ip, const std::string& portStr)
{
	int port;
	sscanf(portStr.c_str(), "%d", &port);

	memset(&_sockaddrIn, 0, sizeof(_sockaddrIn));
	_sockaddrIn.sin_family = AF_INET;
	_sockaddrIn.sin_port = htons(port);
	_sockaddrIn.sin_addr.s_addr = inet_addr(ip.c_str());
	_size = sizeof(_sockaddrIn);

	_address = ip + ":" + portStr;
}

std::string ServersFamily::getAddress()
{
	return _address;
}

void ServersFamily::addServer(const Server &server)
{
	_servers.push_back(server);
}

sockaddr* ServersFamily::getSockAddr()
{
	return reinterpret_cast<sockaddr *>(&_sockaddrIn);
}

socklen_t* ServersFamily::getSockAddrSize()
{
	return &_size;
}
