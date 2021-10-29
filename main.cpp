#include "Webserver.class/Webserver.hpp"

int main(int argc, char** argv) {
	std::string conf_path;
	if (argc == 1)
		conf_path = DEFAULT_CONFIG_PATH;
	else if (argc == 2)
		conf_path = argv[1];
	else
		return 1;
	std::vector<Server> hosts = Parser::parse(conf_path);
	Webserver *webserver = new Webserver(&hosts);
	webserver->start();
	return 0;
}
