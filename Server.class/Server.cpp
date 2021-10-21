#include "Server.hpp"
#include <fstream>

# include "../IHTTPMessage.interface/Response.class/errorResponse.hpp"
# include "../IHTTPMessage.interface/Request.class/Request.hpp"

Server::Server(const Server::connection_struct &connectionStruct) : error_(1), structManager(connectionStruct) {
	socketInit();
	socketBind();
	socketListening();
}

void Server::start() {
	Debug::Log("Start server");

	selectHelper.setMaster(socket_);

	for (;selectHelper.getCount();) {           //////////////////////////////////////????????
		selectHelper.makeSelect();
		FDBeginner();
	}
}

void Server::FDBeginner() {
	for (int i = 0; i <= selectHelper.getCount(); i++) {
		if (selectHelper.isUtils(i)) {			/////////////////////////////////////????????????????
			if (i == socket_) {
				doAccept();
			} else {
				doRead(i);
			}
		}
	}
}

void Server::doAccept() {
	int acceptFD;
	Debug::Log("new connect");
	acceptFD = accept(socket_, reinterpret_cast<sockaddr*>(structManager.getAddress()), structManager.getSize());
	if (acceptFD == -1) {
		throw Server::ServerException("no accept");
	} else {
		selectHelper.setMaster(acceptFD);
	}
}

void Server::doRead(int &socket) {

//    const unsigned int MAX_BUF_LENGTH = 4096;
//    std::vector<char> buffer(MAX_BUF_LENGTH);
//    std::string receivedString;
//    int bytesReceived_ = 0;
//    do {
//        bytesReceived_ = recv(socket, &buffer[0], buffer.size(), 0);
//        if (bytesReceived_ <= 0) {
//            if (bytesReceived_ == 0) {
//                std::string mes = std::to_string(socket) + " close connect";
//                Debug::Log(mes, true);
//                doWrite(socket, mes.c_str());
//            } else
//                Debug::Log("no read", true);
//            close(socket);
//            selectHelper.deleteMaster(socket);
//        } else {
//            receivedString.append(buffer.cbegin(), buffer.cend());
//        }
//    } while (bytesReceived_ == MAX_BUF_LENGTH);
//    if (bytesReceived_ > 0)
//    {
//        std::cout << receivedString << std::endl;
//        Request req(receivedString);
////        if (req.getResponseType() == 200)
////        {
////
////        }
//        Response resp(req.getResponseType(), req.getStartLine(), req.getHeaders(), req.getBodies());
//        doWrite(socket, resp.getResponse());
//    }

	char buf[1024];
	std::memset(buf, 0, 1024);
	error_ = recv(socket, buf, 1024, 0);
	if (error_ <= 0) {
		if (error_ == 0) {
			std::string mes = std::to_string(socket) + " close connect";
			Debug::Log(mes, true);
			doWrite(socket, mes.c_str());
		}
		else
			Debug::Log("no read", true);
		close(socket);
		selectHelper.deleteMaster(socket);
	} else {
        const std::string buffer  = std::string(buf);
//        std::cout << buffer;
        Request req(buffer);
        Response resp(req.getResponseType(), req.getStartLine(), req.getHeaders(), req.getBodies());

		doWrite(socket, resp.getResponse());
	}
}

//int Server::sendAll(int &socket,  const std::string & buf, int bufLength)
//{
//    int totalBytesSend = 0;
//    int bytesLeftToSend = bufLength; // сколько байт осталось послать
//    int chunk = 0;
//    while (totalBytesSend < bufLength) {
//        chunk = send(socket, &buf[totalBytesSend], bytesLeftToSend, 0);
//        if (chunk == -1) { return -1; }
//        totalBytesSend += chunk;
////        bytesLeftToSend -= chunk;
//    }
//    return 0;
//}

void Server::doWrite(int &socket, const std::string & buf) {

    if (selectHelper.isMaster(socket)) {
            error_ = send(socket, &buf[0], buf.size(), 0);
            if (error_ == -1) {
                Debug::Log("no write", true);
        }
    }

//	for(int j = 0; j <= selectHelper.getCount(); j++) {
//		if (selectHelper.isMaster(j)) {
//			if (j != socket_ && j != socket) {
//				error_ = send(j, cBuf.c_str(), cBuf.size(), 0);
//				if (error_ == -1) {
//					Debug::Log("no write", true);
//				}
//			}
//		}
//	}
}

void Server::socketBind() {
	setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &error_, sizeof(int));

	error_ = bind(socket_, reinterpret_cast<sockaddr *>((structManager.getAddress())), *structManager.getSize());
	if (error_ == -1) {
		throw Server::ServerException("no bind");
	}
}

void Server::socketListening() {
        error_ = listen(socket_, 10);
        if (error_ == -1) {
		throw Server::ServerException("no listen");
	}
}

void Server::socketInit() {
	socket_ = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(socket_, F_SETFL, O_NONBLOCK);
	if (socket_ == -1) {
		throw Server::ServerException("no socket");
	}
}
