#include "Webserver.hpp"
#include "../IHTTPMessage.interface/Request.class/Request.hpp"
#include "../IHTTPMessage.interface/Response.class/Response.class.hpp"

Webserver::Webserver(std::vector<ServersFamily>* Families)
{
	families = Families;
	error_ = 0;
	server_run = true;
	compress_ = true;
	timeout_ = DEFAULT_TIMEOUT;
	createSockets();
}
Webserver::~Webserver() {}

void Webserver::createSockets()
{
	std::vector<ServersFamily>::iterator it = families->begin();
	for (; it != families->end(); it++)
	{
		int sock = socketInit();
		socketReusable(sock);
		socketBind(sock, *it);
		socketListening(sock);
		pollStruct.addListener(sock);
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

void Webserver::socketBind(int sock, ServersFamily& family)
{
	error_ = fcntl(sock, F_SETFL, O_NONBLOCK);
	if (error_ < 0) {
		// close(sock);
		pollStruct.cleanUpSockets();
//		throw Server::ServerException("fcntl() failed");
		exit(1);
	}

	error_ = bind(sock, family.getSockAddr(), *(family.getSockAddrSize()));
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
//		close(sock);
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
		handleEvent();
	}
	pollStruct.cleanUpSockets();
}

void Webserver::handleEvent()
{
	for (int i = 0; i < pollStruct.getCount(); i++) {
		if (pollStruct.getRevents(i) == 0)
			continue;
		else if ((pollStruct.getRevents(i) & POLLIN) != POLLIN) {
			server_run = false;
			Debug::Log("no pollin", true);
			break;  ///////// throw?
		}
		if (pollStruct.isListenSocket(i))
			doAccept(i);
		else {
			handleConnection(i);
		}
		if (compress_) {  ///////////////////////Peredelat'
			compress_ = false;
			pollStruct.compress();
		}
	}
}

void Webserver::doAccept(int i)
{
	int acceptFD;
	Debug::Log("new connect");
	int sock = pollStruct.getSocket(i);
	ServersFamily family = (*families)[i];
	acceptFD = accept(sock, family.getSockAddr(), family.getSockAddrSize());
	if (acceptFD < 0) {
		server_run = false;
		Debug::Log("no accept", true);
//		throw Server::ServerException("no accept");
		exit(1);
	} else {
		pollStruct.addConection(acceptFD);
	}
}
//
//Server &Webserver::getServerByIndex(int index)
//{
//	std::vector<Server>::iterator it = hosts->begin();
//	for (; it != hosts->end(); it++)
//	{
//		if (it->getIndex() == index)
//			return *it;
//	}
//	exit(1);
//}


void Webserver::handleConnection(int i)
{
	int socket = pollStruct.getSocket(i);
	int closeConnection = doRead(socket);
	////////////////////////
	Request req(buffer);
//	req.getHeaders().headers;
	Server serv = (*families)[1].getServerByName("HOST FIELD FROM REQUEST HEADER");
	Response resp(req.getStatusCode(), req.getStartLine(), req.getHeaders(), req.getBodies(), &serv);
	doWrite(socket, resp.getResponse());
	if (closeConnection) {
		pollStruct.closeConnection(i);
		compress_ = true;
	}
}

int Webserver::doRead(int socket)
{
	char buf[1024];
	std::memset(buf, 0, 1024);
	error_ = recv(socket, buf, 1024, 0);
	if (error_ <= 0) {
		if (error_ == 0) {
			std::string mes = std::to_string(socket) + " close connect";
			Debug::Log(mes, true);
			//            PollStruct::closeConnection(?);
		} else
			Debug::Log("recv() failed", true);
		return 1;
	} else {
		buffer = std::string(buf);
	}
	return 0;}

void Webserver::doWrite(int socket, const std::string &buf)
{
	error_ = send(socket, &buf[0], buf.size(), 0);
	if (error_ == -1) {
		Debug::Log("no write", true);
	}
}


