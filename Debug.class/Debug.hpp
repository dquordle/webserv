#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <string>

class Debug {
public:
	static void Log(const std::string &log, bool isError = false);
};

#endif
