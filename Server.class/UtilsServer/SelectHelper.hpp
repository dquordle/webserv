#ifndef SELECTHELPER_HPP
#define SELECTHELPER_HPP

#include <sys/select.h>
#include "../ServerException/ServerException.hpp"

class SelectHelper {
private:
	int 				countFD_;
	fd_set				master_FD_;
	fd_set				utils_FD_;
public:
	typedef ServerException		ServerException;
public:
	SelectHelper();

	void setMaster(const int &socket);

	void deleteMaster(const int &socket);

	void makeSelect();

	bool isUtils(const int &socket) const;

	bool isMaster(const int &socket) const;

	int getCount() const;
};

#endif
