#include "Server.hpp"
#include <fstream>

# include "../IHTTPMessage.interface/Response.class/errorResponse.hpp"
# include "../IHTTPMessage.interface/Request.class/Request.hpp"

Server::Server(const Server::connection_struct &connectionStruct) : error_(1), structManager(connectionStruct), server_run(1) {
	socketInit(connectionStruct);
	socketReusable();
	socketBind();
	socketListening();
	PollStruct.initialize(socket_);
	timeout_ = (10 * 60 * 1000);
	compress = 0;
}

void Server::start() {
	Debug::Log("Start server");

	for (;server_run;) {
		PollStruct.makePoll(timeout_);
		FDBeginner();
	}
	PollStruct.cleanUpSockets();
}

void Server::FDBeginner() {
	for (int i = 0; i < PollStruct.getCount(); i++) {
	    if (PollStruct.getRevents(i) == 0) continue;
	    else if ((PollStruct.getRevents(i) & POLLIN) != POLLIN) {
	        server_run = 0;
	        Debug::Log("no pollin", true);
	        break;
	    }
        if (PollStruct.isListenSocket(i))
            doAccept();
        else {
            handleConnection(i);
        }
        if (compress) {
            compress = 0;
            PollStruct.compress();
        }

	}
}

void Server::doAccept() {
//    for (int acceptFD = 1; acceptFD != -1; ) {
        int acceptFD;
        Debug::Log("new connect");
        acceptFD = accept(socket_, reinterpret_cast<sockaddr*>(structManager.getStruct()), structManager.getSize());
        if (acceptFD < 0) {
            server_run = 0;
            Debug::Log("no accept", true);
//            break;
            throw Server::ServerException("no accept");
        } else {
            PollStruct.addConection(acceptFD);
        }
//    }
}

void Server::handleConnection(int &i) {
    int socket = PollStruct.getSocket(i);
    int closeConnection = doRead(socket);
    Request req(buffer);
    Response resp(req.getStatusCode(), req.getStartLine(), req.getHeaders(), req.getBodies());
    doWrite(socket, resp.getResponse());
    if (closeConnection) {
        PollStruct.closeConnection(i);
        compress = 1;
    }
}

int Server::doRead(int &socket) {
    char buf[1024];
    std::memset(buf, 0, 1024);
    error_ = recv(socket, buf, 1024, 0);
    if (error_ <= 0) {
        if (error_ == 0) {
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
    error_ = send(socket, &buf[0], buf.size(), 0);
    if (error_ == -1) {
        Debug::Log("no write", true);
    }
}

void Server::socketListening() {
    error_ = listen(socket_, 10);
    if (error_ == -1) {
        close(socket_);
        throw Server::ServerException("no listen");
    }
}

void Server::socketBind() {
    error_ = fcntl(socket_, F_SETFL, O_NONBLOCK);
    if (error_ < 0) {
        close(socket_);
        throw Server::ServerException("fcntl() failed");
    }

	error_ = bind(socket_, reinterpret_cast<sockaddr *>((structManager.getStruct())), *structManager.getSize());
	if (error_ == -1) {
	    close(socket_);
		throw Server::ServerException("no bind");
	}
}

void Server::socketReusable() {
    error_ = setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (char *)&flagsOn_, sizeof(int));

    if (error_ < 0) {
        close(socket_);
        throw Server::ServerException("setsockopt() failed");
    }
}

void Server::socketInit(const Server::connection_struct &connectionStruct) {
	socket_ = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_ == -1) {
		throw Server::ServerException("no socket");
	}
}
