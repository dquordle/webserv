#include "PollStruct.hpp"

PollStruct::PollStruct() {
    memset(fds_, 0, sizeof(fds_));
    nfds_ = 1;
}

void PollStruct::initialize(const int &socket) {
    listen_sd = socket;
    fds_[0].fd = listen_sd;
    fds_[0].events = POLLIN;
}

void PollStruct::addConection(const int &new_socket) {
    fds_[nfds_].fd = new_socket;
    fds_[nfds_].events = POLLIN;
    nfds_++;
}

void PollStruct::makePoll(int timeout) {
    int ret;

    ret = poll(fds_, nfds_, timeout);
	if (ret < 0) {
	    throw PollStruct::ServerException("poll() failed");
	}
	if (ret == 0) {
//	    TODO: настроить poll если time out
        Debug::Log("timeout", true);
        throw PollStruct::ServerException("poll() timeout");
	}
	current_size_ = nfds_;
}

int PollStruct::getRevents(int i) const {
    return (fds_[i].revents);
}

bool PollStruct::isListenSocket(const int &i) const {
	return fds_[i].fd == listen_sd;
}

void PollStruct::cleanUpSockets() {
    for (int i = 0; i < nfds_; i++) {
        if (fds_[i].fd >= 0)
            close(fds_[i].fd);
    }
}

void PollStruct::closeConnection(int i) {
    close(fds_[i].fd);
    fds_[i].fd = -1;
}

int PollStruct::getCount() const  {
	return current_size_;
}

int PollStruct::getSocket(int &i) const  {
    return fds_[i].fd;
}

void PollStruct::compress() {
    for (int i = 0; i < nfds_; i++) {
        if (fds_[i].fd == -1) {
            for (int j = i; j < nfds_; j++) {
                fds_[j].fd = fds_[j + 1].fd;
            }
            i--;
            nfds_--;
        }
    }
}