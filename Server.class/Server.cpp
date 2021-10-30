#include "Server.hpp"
#include <fstream>

# include "../IHTTPMessage.interface/Response.class/errorResponse.hpp"
# include "../IHTTPMessage.interface/Request.class/Request.hpp"

Server::Server(const Server::connection_struct &connectionStruct, Host * host) : _error(1), structManager(connectionStruct), _server_run(1) {
    try {
        socketInit();
        socketReusable();
        socketBind();
        socketListening();
        _PollStruct.initialize(_socket);
    } catch (std::exception &ex)
    {
        Debug::Log(ex.what());
    }
	_timeout = (10 * 60 * 1000);
	_compress = 0;
	_host = host;
}

void Server::start() {
	Debug::Log("Start server");

	for (;_server_run;) {
		_PollStruct.makePoll(_timeout);
		FDBeginner();
	}
	_PollStruct.cleanUpSockets();
}

void Server::FDBeginner() {
	for (int i = 0; i < _PollStruct.getCount(); i++) {
	    if (_PollStruct.getRevents(i) == 0) continue;
	    else if ((_PollStruct.getRevents(i) & POLLIN) != POLLIN) {
	        _server_run = 0;
	        Debug::Log("no pollin", true);
	        break;
	    }
        if (_PollStruct.isListenSocket(i))
            doAccept();
        else {
            handleConnection(i);
        }
        if (_compress) {
            _compress = 0;
            _PollStruct.compress();
        }

	}
}

void Server::doAccept() {
//    for (int acceptFD = 1; acceptFD != -1; ) {
        int acceptFD;
        Debug::Log("new connect");
        acceptFD = accept(_socket, reinterpret_cast<sockaddr*>(structManager.getStruct()), structManager.getSize());
        if (acceptFD < 0) {
            _server_run = 0;
            Debug::Log("no accept", true);
//            break;
            throw Server::ServerException("no accept");
        } else {
            _PollStruct.addConection(acceptFD);
        }
//    }
}

void Server::handleConnection(int &i) {
    int socket = _PollStruct.getSocket(i);
    int closeConnection = doRead(socket);
    Request req(buffer);
    Response resp(req.getStatusCode(), req.getStartLine(), req.getHeaders(), req.getBodies(), _host);
    doWrite(socket, resp.getResponse());
    if (closeConnection) {
        _PollStruct.closeConnection(i);
        _compress = 1;
    }
}

int Server::doRead(int &socket) {
    char buf[1024];
    std::memset(buf, 0, 1024);
    _error = recv(socket, buf, 1024, 0);
    if (_error <= 0) {
        if (_error == 0) {
            std::string mes = std::to_string(socket) + " close connect";
            Debug::Log(mes, true);
        } else
            Debug::Log("recv() failed", true);
        return 1;
    } else {
        buffer = std::string(buf);
    }
    return 0;
}

void Server::doWrite(int &socket, const std::string & buf) {
    _error = send(socket, &buf[0], buf.size(), 0);
    if (_error == -1) {
        Debug::Log("no write", true);
    }
}

void Server::socketListening() {
    _error = listen(_socket, 10);
    if (_error == -1) {
        close(_socket);
        throw Server::ServerException("no listen");
    }
}

void Server::socketBind() {
    _error = fcntl(_socket, F_SETFL, O_NONBLOCK);
    if (_error < 0) {
        close(_socket);
        throw Server::ServerException("fcntl() failed");
    }

	_error = bind(_socket, reinterpret_cast<sockaddr *>((structManager.getStruct())), *structManager.getSize());
	if (_error == -1) {
	    close(_socket);
		throw Server::ServerException("no bind");
	}
}

void Server::socketReusable() {
    _error = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &_error, sizeof(int));

    if (_error < 0) {
        close(_socket);
        throw Server::ServerException("setsockopt() failed");
    }
}

void Server::socketInit() {
	_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (_socket == -1) {
		throw Server::ServerException("no socket");
	}
}
