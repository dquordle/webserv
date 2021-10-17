#include <iostream>
#include "Parser.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	//	lets get this config parsed
	std::vector<Host> hosts = Parser::parse(argv[1]);
	std::cout << hosts.size() << std::endl;
}