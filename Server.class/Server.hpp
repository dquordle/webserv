#ifndef SERVER_HPP
#define SERVER_HPP

#include <unistd.h>
#include "UtilsServer/StructManager.hpp"
#include "UtilsServer/SelectHelper.hpp"
#include "../Debug.class/Debug.hpp"
#include "ServerException/ServerException.hpp"
#include "../IHTTPMessage.interface/Request.class/Request.hpp"
#include "../IHTTPMessage.interface/Response.class/Response.class.hpp"
#include <fcntl.h>

class Server {
private:
	StructManager		structManager;
	SelectHelper		selectHelper;
	int					error_;
	int 				socket_;
public:
	typedef StructManager::connection_struct	connection_struct;
	typedef ServerException						ServerException;
public:
	explicit Server(const Server::connection_struct &);

	void start();

private:
	void FDBeginner();

	void doAccept();

	void doRead(int &);

	void doWrite (int &, const std::string &);

	void socketBind();

	void socketListening();

	void socketInit(const Server::connection_struct &connectionStruct);

//	int sendAll(int & socket,  const std::string & buf, int bufLength);
};

#endif
