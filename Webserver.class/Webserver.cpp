#include "Webserver.hpp"
#include "../IHTTPMessage.interface/Request.class/Request.hpp"
#include "../IHTTPMessage.interface/Response.class/Response.class.hpp"

Webserver::Webserver(std::vector<ServersFamily>* Families)
{
	families = Families;
	error_ = 0;
	server_run = true;
	compress_ = false;
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
		Debug::FatalError("socket function returned error", &pollStruct);
	}
	return sock;
}

void Webserver::socketReusable(int sock) {
	int flagsOn_;
	error_ = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&flagsOn_, sizeof(int));

	if (error_ < 0) {
		close(sock);
		Debug::FatalError("setsockopt function returned error", &pollStruct);
	}
}

void Webserver::socketBind(int sock, ServersFamily& family)
{
	error_ = fcntl(sock, F_SETFL, O_NONBLOCK);
	if (error_ < 0) {
		close(sock);
		Debug::FatalError("fcntl function returned error", &pollStruct);
	}

	error_ = bind(sock, family.getSockAddr(), *(family.getSockAddrSize()));
	if (error_ == -1) {
		close(sock);
		Debug::FatalError("bind function returned error", &pollStruct);
	}
}

void Webserver::socketListening(int sock) {
	error_ = listen(sock, 10);
	if (error_ == -1) {
		close(sock);
		Debug::FatalError("listen function returned error", &pollStruct);
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
		pollStruct.setReventsZero(i);
		if (compress_) {
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
//		server_run = false;
		Debug::FatalError("accept function returned error", &pollStruct); //////////??????????
	} else {
		pollStruct.addConection(acceptFD, i);
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
	bool closeConnection = doRead(socket);
	////////////////////////
	if (closeConnection) {
		pollStruct.closeConnection(i);
		compress_ = true;
	}
	else {
		Request req(buffer);
		//	req.getHeaders().headers;
		ServersFamily family = (*families)[pollStruct.getListeningIndex(i)];
		Server serv = family.getServerByName("HOST FIELD FROM REQUEST HEADER");
		Response resp(req.getStatusCode(), req.getStartLine(), req.getHeaders(), req.getBodies(), &serv);
		doWrite(socket, resp.getResponse());
		buffer.clear();
	}
}

bool	Webserver::doRead(int socket)
{
	char buf[10240];
	std::memset(buf, 0, 10240);
	error_ = recv(socket, buf, 10240, 0);
	if (error_ <= 0) {
		if (error_ == 0) {
			std::string mes = std::to_string(socket) + " close connect";
			Debug::Log(mes);
		} else
			Debug::Log("recv() failed", true);
		return true;
	} else {
		buffer = std::string(buf);
	}
	return false;
}

void Webserver::doWrite(int socket, const std::string &buf)
{
	error_ = send(socket, &buf[0], buf.size(), 0);
	if (error_ == -1) {
		Debug::Log("no write", true);
	}
}


