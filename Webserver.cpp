#include "Webserver.hpp"

Webserver::Webserver(const std::vector<Host>& Hosts) : hosts(Hosts), error_(0), server_run(true)
{
	createSockets();
}
Webserver::~Webserver() {}

void Webserver::createSockets()
{
	std::vector<Host>::iterator it = hosts.begin();
	for (; it != hosts.end(); it++)
	{
		if (it->isDefault())
		{
			int sock = socketInit();
			socketReusable(sock);
			socketBind(sock, *it);
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
	error_ = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&flagsOn_, sizeof(int));

	if (error_ < 0) {
		// close(sock);
		pollStruct.cleanUpSockets();
//		throw Server::ServerException("setsockopt() failed");
		exit(1);
	}
}

void Webserver::socketBind(int sock, Host& host)
{
	error_ = fcntl(sock, F_SETFL, O_NONBLOCK);
	if (error_ < 0) {
		// close(sock);
		pollStruct.cleanUpSockets();
//		throw Server::ServerException("fcntl() failed");
		exit(1);
	}

//	error_ = bind(sock, reinterpret_cast<sockaddr *>((structManager.getStruct())), *structManager.getSize()); ////
	error_ = bind(sock, host.getSockAddr(), host.getSockAddrSize());
	if (error_ == -1) {
		// close(sock);
		pollStruct.cleanUpSockets();
//		throw Server::ServerException("no bind");
		exit(1);
	}
}

void Webserver::socketListening(int sock) {
	error_ = listen(sock, 10);
	if (error_ == -1) {
		// close(sock);
		pollStruct.cleanUpSockets();
//		throw Server::ServerException("no listen");
		exit(1);
	}
}

void Webserver::start()
{
	Debug::Log("Start server");

	for (;server_run;) {
		pollStruct.makePoll(timeout_);
		FDBeginner();
	}
	pollStruct.cleanUpSockets();
}



