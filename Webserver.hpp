#ifndef WEBSERV_WEBSERVER_HPP
#define WEBSERV_WEBSERVER_HPP

#include <arpa/inet.h>
#include <fcntl.h>
#include "Config_parser/Host.hpp"
#include "Server.class/UtilsServer/PollStruct.hpp"

class Webserver
{

private:
	PollStruct		    pollStruct;
	std::vector<Host>	hosts;
	int					error_;
	bool 				server_run;
	int                 timeout_;
	int                 compress;
	std::string         buffer;


public:
	explicit Webserver(const std::vector<Host>& Hosts);
	~Webserver();
	void start();

private:
	void createSockets();
	int socketInit();
	void socketReusable(int sock);
	void socketBind(int sock, Host& host);
	void socketListening(int sock);

	void FDBeginner();

	void doAccept();

	int doRead(int &);

	void doWrite (int &, const std::string &);

	void handleConnection(int &);
};


#endif
