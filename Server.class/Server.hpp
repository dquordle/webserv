#ifndef SERVER_HPP
#define SERVER_HPP

#include <unistd.h>
#include "UtilsServer/StructManager.hpp"
#include "UtilsServer/PollStruct.hpp"
#include "../Debug.class/Debug.hpp"
#include "ServerException/ServerException.hpp"
#include "../IHTTPMessage.interface/Request.class/Request.hpp"
#include "../IHTTPMessage.interface/Response.class/Response.class.hpp"
#include <fcntl.h>

class Server {
private:
	StructManager		structManager;
	PollStruct		    PollStruct;
	int					error_;
	int 				socket_;
	int					flagsOn_;
	int                 timeout_;
	int                 server_run;
	int                 compress;
	std::string         buffer;
public:
	typedef StructManager::connection_struct	connection_struct;
	typedef ServerException						ServerException;
public:
	explicit Server(const Server::connection_struct &);

	void start();

private:
	void FDBeginner();

	void doAccept();

	int doRead(int &);

	void doWrite (int &, const std::string &);

	void socketBind();

	void socketListening();

	void socketInit(const Server::connection_struct &connectionStruct);

	void socketReusable();

	void handleConnection(int &);

};

#endif
