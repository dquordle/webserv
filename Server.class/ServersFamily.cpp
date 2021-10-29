#include "ServersFamily.hpp"

ServersFamily::ServersFamily(const std::string& ip, std::string portStr)
{
	int port;
	sscanf(portStr.c_str(), "%d", &port);

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

void ServersFamily::setSocket(int socket)
{
	_socket = socket;
}
