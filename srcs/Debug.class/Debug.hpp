#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <string>
#include "../Server.class/UtilsServer/PollStruct.hpp"

class PollStruct;

class Debug {
public:
	static void Log(const std::string &log, bool isError = false);
	static void	FatalError(const std::string &log, PollStruct *pollStruct = NULL);
};

#endif
