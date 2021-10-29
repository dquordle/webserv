#ifndef WEBSERV_WEBSERVER_HPP
#define WEBSERV_WEBSERVER_HPP

#include <arpa/inet.h>
#include <fcntl.h>
#include "../Config_parser/Parser.hpp"
#include "../Server.class/UtilsServer/PollStruct.hpp"
#include "../Configs/Configuration.hpp"

class Webserver
{

private:
	PollStruct		    		pollStruct;
	std::vector<Server>*		hosts;
	int							error_;
	bool 						server_run;
	int                 		timeout_;
	bool                		compress_;
	std::string         		buffer;
//	std::map<int, std::string>	indexMap;


public:
	explicit Webserver(std::vector<Server>* Hosts);
	~Webserver();
	void start();

private:
	void createSockets();
	int socketInit();
	void socketReusable(int sock);
	void socketBind(int sock, Server& host);
	void socketListening(int sock);

	void handleEvent();

	void doAccept(int i);
	Server& getServerByIndex(int index);
	void handleConnection(int i);

	int doRead(int socket);

	void doWrite (int socket, const std::string & buf);

};


#endif
