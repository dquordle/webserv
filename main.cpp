#include "Server.class/Server.hpp"
#include "Config_parser/Host.hpp"
#include "Config_parser/Parser.hpp"

int main(int argc, char** argv) {
	if (argc != 2)
		return 1;
	//	lets get this config parsed
	std::vector<Host> hosts = Parser::parse(argv[1]);

	Server::connection_struct connectionStruct = StructManager::connection_struct(1111, "127.0.0.1");
	Server server(connectionStruct);
	server.start();
	return 0;
}
