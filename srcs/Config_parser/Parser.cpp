#include "Parser.hpp"

std::vector<ServersFamily> Parser::parse(const std::string& config)
{
	std::vector<ServersFamily> families;

	std::ifstream ifs(config);
	if (!ifs.good())
		Debug::FatalError("File error");

	std::string nextLine = superGetNextLine(ifs);
	while (nextLine == "server{")
	{
			Server *server = parseServer(ifs);
			putServerToFamily(&families, *server);
			delete(server);
			nextLine = superGetNextLine(ifs);
	}
	ifs.close();
	if (families.empty())
		Debug::FatalError("Configuration file error");
	return families;
}

void Parser::putServerToFamily(std::vector<ServersFamily> *families, Server &server)
{
	std::string address = server.getIp() + ":" + server.getPortStr();
	std::vector<ServersFamily>::iterator it = (*families).begin();
	for (;it != (*families).end(); it++)
	{
		if (it->getAddress() == address)
		{
			it->addServer(server);
			return;
		}
	}
	ServersFamily family(server.getIp(), server.getPortStr());

	family.addServer(server);
	families->push_back(family);
}

Server * Parser::parseServer(std::ifstream & ifs)
{
	Server *server = new Server();
	std::string str = superGetNextLine(ifs);
	while (!str.empty())
	{
		if (str == "routes{")
			parseRoutes(ifs, *server);
		else if (str == "}")
			return server;
		else
		{
			unsigned long pos = str.find(':');
			if (pos == std::string::npos)
				Debug::FatalError("Configuration file error");
			std::string key = str.substr(0, pos);
			std::string value = str.substr(pos + 1, str.size());
			putFieldIntoHost(*server, key, value);
		}
		str = superGetNextLine(ifs);
	}
	Debug::FatalError("Configuration file error");
    return nullptr;
}

void Parser::putFieldIntoHost(Server & server, const std::string& key, const std::string& value)
{
	if (key == "host")
		server.setIP(value);
	else if (key == "port")
		server.setPort(value);
	else if (key == "server_name")
		server.setServerName(value);
	else if (key == "error_page")
		server.addError(value);
	else
		Debug::FatalError("Configuration file error");
}

void Parser::parseRoutes(std::ifstream &ifs, Server &server)
{
	std::string str = superGetNextLine(ifs);
	if (str.empty())
		Debug::FatalError("Configuration file error");
	while (str != "}")
	{
		std::string start = str.substr(0, 5);
		char end = str.at(str.size() - 1);
		if (start != "route" || end != '{')
			Debug::FatalError("Configuration file error");
		std::string routeName = str.substr(5, str.size() - 6);
		Route *route = parseRoute(ifs, routeName);
		server.addRoute(*route);
		delete(route);
		str = superGetNextLine(ifs);
	}
}

Route *Parser::parseRoute(std::ifstream &ifs, const std::string& name)
{
	if (name.empty())
		Debug::FatalError("Configuration file error");
	Route *route = new Route(name);
	std::string str = superGetNextLine(ifs);
	while (!str.empty())
	{
		if (str == "}")
		{
			route->checkCgi();
			return route;
		}
		unsigned long pos = str.find(':');
		if (pos == std::string::npos)
			Debug::FatalError("Configuration file error");
		std::string key = str.substr(0, pos);
		std::string value = str.substr(pos + 1, str.size());
		putFieldIntoRoute(*route, key, value);
		str = superGetNextLine(ifs);
	}
	Debug::FatalError("Configuration file error");
    return nullptr;
}

void Parser::putFieldIntoRoute(Route &route, const std::string &key, const std::string &value)
{
	if (key == "directory")
		route.setDirectory(value);
	else if (key == "save_path")
		route.setSavePath(value);
	else if (key == "methods_allowed")
		route.setMethods(value);
	else if (key == "redirection")
		route.setRedirection(value);
	else if (key == "autoindex")
		route.setAutoindex(value);
	else if (key == "index_file")
		route.setIndexFile(value);
	else if (key == "cgi_path")
		route.setCgiPath(value);
	else if (key == "cgi_ext")
		route.setCgiExt(value);
	else if (key == "limit_body_size")
		route.setMaxBodySize(value);
	else
		Debug::FatalError("Configuration file error");
}

std::string Parser::superGetNextLine(std::ifstream & ifs)
{
	std::string str;
	for (;std::getline(ifs, str, '\n');)
	{
		std::string::iterator end_pos = std::remove(str.begin(), str.end(), '\t');
		str.erase(end_pos, str.end());
		std::string::iterator end = std::remove(str.begin(), str.end(), ' ');
		str.erase(end, str.end());
		unsigned long pos = str.find('#');
		if (pos != std::string::npos)
			str.erase(pos, str.size());
		if (!str.empty())
			break;
	}
	return str;
}

