#include "Webserver.hpp"

Webserver::Webserver(const std::vector<Host>& Hosts) : hosts(Hosts)
{
	createSockets();
}

void Webserver::createSockets()
{
	std::vector<Host>::iterator it = hosts.begin();
	for (; it != hosts.end(); it++)
	{
		if (it->isDefault())
		{
			int sock = socketInit();
			socketReusable(sock);
			socketBind(sock);
			socketListening(sock);
			pollStruct.addListener(sock);
		}
	}

}

int Webserver::socketInit() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		exit(1);
	}
	return sock;
}

void Webserver::socketReusable(int sock) {
	int flagsOn_;
	int error = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&flagsOn_, sizeof(int));

	if (error < 0) {
		close(sock);
//		throw Server::ServerException("setsockopt() failed");
		exit(1);
	}
}

void Webserver::socketBind(int sock) {
	int error = fcntl(sock, F_SETFL, O_NONBLOCK);
	if (error < 0) {
		close(sock);
//		throw Server::ServerException("fcntl() failed");
		exit(1);
	}

//	error = bind(sock, reinterpret_cast<sockaddr *>((structManager.getStruct())), *structManager.getSize()); ////
//	if (error_ == -1) {
//		close(socket_);
//		throw Server::ServerException("no bind");
//	}
}

void Webserver::socketListening(int sock) {
//	error_ = listen(socket_, 10);
//	if (error_ == -1) {
//		close(socket_);
//		throw Server::ServerException("no listen");
//	}
}





