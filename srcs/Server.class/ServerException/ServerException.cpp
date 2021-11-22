#include "ServerException.hpp"

const char *ServerException::what() const throw() {
	std::string ret = "Server Error: " + logLine_;
	return ret.c_str();
}

ServerException::ServerException(const std::string &log) {
	logLine_ = log;
}

ServerException::~ServerException() _NOEXCEPT {}
