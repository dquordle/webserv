#include "PollStruct.hpp"

PollStruct::PollStruct() {
    memset(fds_, 0, sizeof(fds_));
    nfds_ = 0;
	max_listen_sd = 0;
}

void PollStruct::addListener(int socket) {
    fds_[nfds_].fd = socket;
    fds_[nfds_].events = POLLIN;
    max_listen_sd = nfds_++;
}

void PollStruct::addConection(const int new_socket, const int index) {
    fds_[nfds_].fd = new_socket;
    fds_[nfds_].events = POLLIN;
    indexMap[nfds_] = index;
    nfds_++;
}

void PollStruct::makePoll(int timeout) {
    int ret;

    ret = poll(fds_, nfds_, timeout);
	if (ret < 0) {
//	    throw PollStruct::ServerException("poll() failed");
		Debug::FatalError("poll function returned error", this);
	}
	if (ret == 0) {
//	    TODO: настроить poll если time out
        Debug::Log("timeout", true);
        Debug::FatalError("poll function timed out", this);
//        throw PollStruct::ServerException("poll() timeout");
	}
	current_size_ = nfds_;
}

int PollStruct::getRevents(int i) const {
    return (fds_[i].revents);
}

bool PollStruct::isListenSocket(const int i) const {
//	return fds_[i].fd == listen_sd;
	return i <= max_listen_sd;
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

int PollStruct::getSocket(int i) const  {
    return fds_[i].fd;
}

void PollStruct::compress() {
    for (int i = 0; i < nfds_; i++) {
        if (fds_[i].fd == -1) {
        	fds_[i].fd = fds_[nfds_ - 1].fd;
        	indexMap[i] = indexMap[nfds_ - 1];
            i--;
            nfds_--;
        }
    }
}

void PollStruct::setReventsZero(int index)
{
	fds_[index].revents = 0;
}

int PollStruct::getListeningIndex(int i)
{
	return indexMap[i];
}
