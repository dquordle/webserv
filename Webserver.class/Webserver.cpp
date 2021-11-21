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
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &error_, sizeof(int));

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
	error_ = listen(sock, 1000);
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
		else if ((pollStruct.getRevents(i) & POLLIN) != POLLIN &&
				(pollStruct.getRevents(i) & POLLOUT) != POLLOUT) {
			server_run = false;
			Debug::Log("SOCKET " + std::to_string(i));
			for (int j = 0; j < pollStruct.getCount(); j++) {
				Debug::Log("socket " + std::to_string(pollStruct.getSocket(j)) + " events " + std::to_string(pollStruct.getEvents(j)) + " revents " + std::to_string(pollStruct.getRevents(i)));
			}
			Debug::FatalError("no pollin and no polout", &pollStruct);
		}
		if (pollStruct.isListenSocket(i))
			doAccept(i);
		else if ((pollStruct.getRevents(i) & POLLOUT) == POLLOUT)
			doWrite(i);
		else {
			handleConnection(i);
		}
		pollStruct.setReventsZero(i);
		if (compress_) {
			compress_ = false;
			pollStruct.compress();
			i--;
		}
	}
}

void Webserver::doAccept(int i)
{
	int acceptFD;
	int sock = pollStruct.getSocket(i);
	ServersFamily family = (*families)[i];
	acceptFD = accept(sock, family.getSockAddr(), family.getSockAddrSize());
	if (acceptFD < 0) {
		Debug::FatalError("accept function returned error", &pollStruct);
	} else {
		pollStruct.addConection(acceptFD, i);
	}
}

void Webserver::handleConnection(int i)
{
	int socket = pollStruct.getSocket(i);
	bool closeConnection = doRead(socket);
	if (closeConnection) {
		pollStruct.closeConnection(i);
		compress_ = true;
		requests[socket].clear();
		return;
	}
	if (requestIsFull(requests[socket]))
	{
		if (requests[socket].find("Transfer-Encoding: chunked") != std::string::npos)
			transferDecoding(socket);
		Request req(requests[socket]);
		ServersFamily family = (*families)[pollStruct.getListeningIndex(i)];
		Server serv = family.getServerByName(req.getHost());
		Response resp(req.getStatusCode(), req.getStartLine(), req.getHeaders(), req.getBodies(), &serv);
		responses[socket] = resp.getResponse();
		pollStruct.switchPolloutEvent(i);
		requests[socket].clear();
	}
}

bool	Webserver::doRead(int socket)
{
	char buf[BUFFER_SIZE];
	std::memset(buf, 0, BUFFER_SIZE);
	error_ = recv(socket, buf, BUFFER_SIZE - 1, 0);
	if (error_ <= 0) {
		if (error_ == 0) {
			std::string mes = std::to_string(socket) + " close connect";
			requests[socket].clear();
			responses[socket].clear();
		} else
			Debug::Log("recv() failed", true);
		return true;
	} else {
		requests[socket].append(buf);
	}
	return false;
}

void Webserver::doWrite(int ind)
{
	int socket = pollStruct.getSocket(ind);
	std::string buf = responses[socket];
	error_ = send(socket, &buf[0], buf.size(), 0);
	if (error_ == -1) {
		Debug::Log("no write", true);
	}
	if (error_ < buf.size())
	{
		responses[socket].erase(0, error_);
		pollStruct.switchPolloutEvent(ind);
	}
	else
		pollStruct.switchPolloutEvent(ind, false);
}

bool Webserver::requestIsFull(const std::string& request)
{
	size_t headerEnd = request.find("\r\n\r\n");
	if (headerEnd == std::string::npos)
		return false;
	size_t clKey = request.find("Content-Length");
	if (clKey != std::string::npos)
	{
		size_t clStart = request.find(':', clKey) + 1;
		size_t clEnd = request.find("\r\n", clStart);
		int clValue;
		sscanf(request.substr(clStart, clEnd - clStart).c_str(), "%d", &clValue);
		if (request.length() - headerEnd - 4 < clValue)
			return false;
	}
	size_t teKey = request.find("Transfer-Encoding: chunked");
	if (teKey != std::string::npos)
	{
		if (request.find("0\r\n\r\n") == std::string::npos)
			return false;
	}
	return true;
}

void Webserver::transferDecoding(int socket)
{
	size_t headerEnd = requests[socket].find("\r\n\r\n");
	std::string res = requests[socket].substr(0, headerEnd + 4);
	size_t prev_pos = headerEnd + 4;
	size_t new_pos = requests[socket].find("\r\n", prev_pos);
	std::string curSize = requests[socket].substr(prev_pos, new_pos - prev_pos);
	while (curSize != "0")
	{
		unsigned int x;
		std::stringstream ss;
		ss << std::hex << curSize;
		ss >> x;
		res.append(requests[socket].substr(new_pos + 2, x));
		prev_pos = new_pos + 4 + x;
		new_pos = requests[socket].find("\r\n", prev_pos);
		curSize = requests[socket].substr(prev_pos, new_pos - prev_pos);
	}
	requests[socket] = res;
}


