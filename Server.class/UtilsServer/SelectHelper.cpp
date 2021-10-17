#include "SelectHelper.hpp"

SelectHelper::SelectHelper() : countFD_(0) {
	FD_ZERO(&master_FD_);
	FD_ZERO(&utils_FD_);
}

void SelectHelper::setMaster(const int &socket) {
	FD_SET(socket, &master_FD_);
	if (countFD_ < socket) {
		countFD_ = socket;
	}
}

void SelectHelper::deleteMaster(const int &socket) {
	FD_CLR(socket, &master_FD_);
}

void SelectHelper::makeSelect() {
	utils_FD_ = master_FD_;				/////////////////////////////////???????????????
	if (select(countFD_ + 1, &utils_FD_, 0, 0, 0) == -1) {
		throw SelectHelper::ServerException("no select");
	}
}

bool SelectHelper::isUtils(const int &socket) const {
	return FD_ISSET(socket, &utils_FD_);
}

bool SelectHelper::isMaster(const int &socket) const {
	return FD_ISSET(socket, &master_FD_);
}

int SelectHelper::getCount() const  {
	return countFD_;
}