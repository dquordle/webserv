//#include "Server.class/Server.hpp"
//#include "Config_parser/Host.hpp"
//#include "Config_parser/Parser.hpp"
#include "Webserver.class/Webserver.hpp"

int main(int argc, char** argv) {
	std::string conf_path;
	if (argc == 1)
		conf_path = DEFAULT_CONFIG_PATH;
	else if (argc == 2)
		conf_path = argv[1];
	else
		return 1;
	//	lets get this config parsed
	std::vector<Host> hosts = Parser::parse(conf_path);
	Webserver *webserver = new Webserver(&hosts);
	webserver->start();
//	Server::connection_struct connectionStruct = StructManager::connection_struct(1111, "127.0.0.1");
//	Server server(connectionStruct);
//	server.start();
	return 0;
}
