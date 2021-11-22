#ifndef POLLSTRUCT_HPP
#define POLLSTRUCT_HPP

#include <sys/poll.h>
#include <cstring>
#include "../ServerException/ServerException.hpp"
#include <unistd.h>
#include "../../Debug.class/Debug.hpp"
#include <map>

class Debug;

class PollStruct {
private:
	struct  pollfd fds_[FD_SETSIZE];
	int     nfds_;
	int     current_size_;
	int     max_listen_sd;
	std::map<int, int>	indexMap;
public:
	typedef ServerException		ServerException;
public:
	PollStruct();

	void addListener(int socket);

	int getRevents(int) const;

	void addConection(const int, const int);

	void compress();

	void makePoll(int);

	bool isListenSocket(const int) const;

	void cleanUpSockets();

	int getCount() const;

	int getSocket(int) const;

	void closeConnection(int i);

	void setReventsZero(int index);

	void switchPolloutEvent(int index, bool set = true);

	int	getListeningIndex(int i);

	int getEvents(int i);

private:

};

#endif
