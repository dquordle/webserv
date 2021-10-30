#ifndef WEB_SERVER_PARSER_HPP
#define WEB_SERVER_PARSER_HPP

#include <fstream>
#include <iostream>
#include "../Server.class/ServersFamily.hpp"

class Parser
{
public:
	static std::vector<ServersFamily> parse(const std::string& config);

private:
	static std::string superGetNextLine(std::ifstream & ifs);
	static Server* parseServer(std::ifstream & ifstream);
	static void putServerToFamily(std::vector<ServersFamily> *family, Server & server);
	static void putFieldIntoHost(Server & server, const std::string& key, const std::string& value);
	static void parseRoutes(std::ifstream & ifstream, Server & server);
	static Route* parseRoute(std::ifstream & ifstream, const std::string& name);
	static void putFieldIntoRoute(Route & route, const std::string& key, const std::string& value);
};


#endif
