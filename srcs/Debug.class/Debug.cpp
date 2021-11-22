#include "Debug.hpp"

void Debug::Log(const std::string &log, bool isError) {
	if (isError) {
		std::cerr << log << std::endl;
	} else {
		std::cout << log << std::endl;
	}
}

void Debug::FatalError(const std::string &log, PollStruct *pollStruct)
{
	Log(log,true);
	if (pollStruct != NULL)
		pollStruct->cleanUpSockets();
	exit(1);
}
