#ifndef STRUCTMANAGER_HPP
#define STRUCTMANAGER_HPP

#include <arpa/inet.h>
#include <string>

class StructManager {
private:
	struct sockaddr_in	serverAddress_;
	socklen_t 			size_;
public:
	struct connection_struct {
		int port;
		std::string ip;
		connection_struct(const int &port, const std::string &ip);
	};
public:
	explicit StructManager(const connection_struct &);

	struct sockaddr_in *getAddress();

	socklen_t	*getSize();
private:
	void updateSize();
};

#endif
