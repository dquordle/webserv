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
#include "../Config_parser/Host.hpp"

class Server {
private:
	StructManager		structManager;
	PollStruct		    _PollStruct;
	int					_error;
	int 				_socket;
	int                 _timeout;
	int                 _server_run;
	int                 _compress;
	std::string         buffer;
	Host *				_host;
public:
	typedef StructManager::connection_struct	connection_struct;
	typedef ServerException						ServerException;
public:
	explicit Server(const Server::connection_struct &, Host *);

	void start();

private:
	void FDBeginner();

	void doAccept();

	int doRead(int &);

	void doWrite (int &, const std::string &);

	void socketBind();

	void socketListening();

	void socketInit();

	void socketReusable();

	void handleConnection(int &);

};

#endif
