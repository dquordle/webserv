#include "Debug.hpp"

void Debug::Log(const std::string &log, bool isError) {
	if (isError) {
		std::cerr << log << std::endl;
	} else {
		std::cout << log << std::endl;
	}
}