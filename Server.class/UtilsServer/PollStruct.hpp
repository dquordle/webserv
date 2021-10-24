#ifndef POLLSTRUCT_HPP
#define POLLSTRUCT_HPP

#include <sys/poll.h>
#include <cstring>
#include "../ServerException/ServerException.hpp"
#include <unistd.h>
#include "../../Debug.class/Debug.hpp"

class PollStruct {
private:
	struct  pollfd fds_[200];
	int     nfds_;
	int     current_size_; //?
	int     max_listen_sd;
public:
	typedef ServerException		ServerException;
public:
	PollStruct();

	int addListener(int socket);

	int getRevents(int) const;

	void addConection(const int &);

	void compress();

	void makePoll(int);

	bool isListenSocket(const int &) const;

	void cleanUpSockets();

	int getCount() const;

	int getSocket(int &) const;

	void closeConnection(int i);
};

#endif
