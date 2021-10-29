#ifndef SERVERSFAMILY_HPP
#define SERVERSFAMILY_HPP

#include "Server.hpp"
class ServersFamily
{
private:
	std::vector<Server>	_servers;
//	std::string			_ip;
//	std::string			_portStr;
//	int 				_port;
	std::string			_address;
	int					_socket;
	struct sockaddr_in	_sockaddrIn;
	socklen_t			_size;

public:
	explicit 	ServersFamily(const std::string& ip, std::string portStr);
	std::string getAddress();
	void		addServer(const Server & server);
	void 		setSocket(int socket);

};


#endif
