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
	PollStruct		    			pollStruct;
	std::vector<ServersFamily>*		families;
	ssize_t 						error_;
	bool 							server_run;
	int                 			timeout_;
	bool                			compress_;
	std::map<int, std::string>		requests;
	std::map<int, std::string>		responses;



public:
	explicit Webserver(std::vector<ServersFamily>* Families);
	~Webserver();
	void start();

private:
	void	createSockets();
	int		socketInit();
	void	socketReusable(int sock);
	void	socketBind(int sock, ServersFamily& family);
	void	socketListening(int sock);

	void	handleEvent();

	void	doAccept(int i);
	void	handleConnection(int i);

	bool	doRead(int socket);

	void	doWrite(int socket, const std::string & buf);

	bool	requestIsFull(const std::string& request);
	void	transferDecoding(int socket);

	void	doWrite(int socket);
};


#endif
