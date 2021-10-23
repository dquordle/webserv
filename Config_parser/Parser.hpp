#ifndef WEB_SERVER_PARSER_HPP
#define WEB_SERVER_PARSER_HPP

#include "Host.hpp"
#include <fstream>
#include <iostream>

const std::string DEFAULT_CONFIG_PATH = "../g_doggy.conf";

class Parser
{
public:
	static std::vector<Host> parse(const std::string& config);

private:
	static std::string superGetNextLine(std::ifstream & ifs);
	static Host* parseServer(std::ifstream & ifstream);
	static void putFieldIntoHost(Host & host, const std::string& key, const std::string& value);
	static void parseRoutes(std::ifstream & ifstream, Host & host);
	static Route* parseRoute(std::ifstream & ifstream, const std::string& name);
	static void putFieldIntoRoute(Route & route, const std::string& key, const std::string& value);
	static void finishInitialization(std::vector<Host> hosts);

};


#endif
