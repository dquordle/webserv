#ifndef SERVERSFAMILY_HPP
#define SERVERSFAMILY_HPP

#include "Server.hpp"
class ServersFamily
{
private:
	std::vector<Server>	_servers;
	std::string			_address;
	struct sockaddr_in	_sockaddrIn;
	socklen_t			_size;

public:
	explicit 	ServersFamily(const std::string& ip, const std::string& portStr);
	std::string getAddress();
	void		addServer(const Server & server);
	sockaddr*	getSockAddr();
	socklen_t* 	getSockAddrSize();
};


#endif
