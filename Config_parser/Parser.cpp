#include "Parser.hpp"

std::vector<ServersFamily> Parser::parse(const std::string& config)
{
	std::ifstream ifs(config);
	std::vector<ServersFamily> families;

	if (!ifs.good())
	{
		std::cout << "File error" << std::endl;
		exit(1);         ///////////// Errors.file_error();
	}
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
		exit(1);
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
	server.setDefault(true);
	family.addServer(server);
	families->push_back(family);
}

Server * Parser::parseServer(std::ifstream & ifs)
{
	Server *host = new Server();
	std::string str = superGetNextLine(ifs);
	while (!str.empty())
	{
		if (str == "routes{")
			parseRoutes(ifs, *host);
		else if (str == "}")
		{
			return host;
			//the end
		}
		else
		{
			unsigned long pos = str.find(':');
			if (pos == std::string::npos)
				exit(1); //////////_error
			std::string key = str.substr(0, pos);
			std::string value = str.substr(pos + 1, str.size());
			putFieldIntoHost(*host, key, value);
		}
		str = superGetNextLine(ifs);
	}
	exit(1);
}

void Parser::putFieldIntoHost(Server & host, const std::string& key, const std::string& value)
{
	if (key == "host")
		host.setIP(value);
	else if (key == "port")
		host.setPort(value);
	else if (key == "server_name")
		host.setServerName(value);
	else if (key == "error_page")	////////////////////////////////////////////
		host.addError(value);
	else if (key == "limit_body_size")
		host.setMaxBodySize(value);
	else
		exit(1);
}

void Parser::parseRoutes(std::ifstream &ifs, Server &host)
{
	std::string str = superGetNextLine(ifs);
	if (str.empty())
		exit(1);
	while (str != "}")
	{
		std::string start = str.substr(0, 5);
		char end = str.at(str.size() - 1);
		if (start != "route" || end != '{')
			exit(1);
		std::string routeName = str.substr(5, str.size() - 6);
		Route *route = parseRoute(ifs, routeName);
		host.addRoute(*route);
		delete(route);
		str = superGetNextLine(ifs);
	}
}

Route *Parser::parseRoute(std::ifstream &ifs, const std::string& name)
{
	if (name.empty())
		exit(1);
	Route *route = new Route(name);
	std::string str = superGetNextLine(ifs);
	while (!str.empty())
	{
		if (str == "}")
			return route;
		unsigned long pos = str.find(':');
		if (pos == std::string::npos)
			exit(1); //////////_error
		std::string key = str.substr(0, pos);
		std::string value = str.substr(pos + 1, str.size());
		putFieldIntoRoute(*route, key, value);
		str = superGetNextLine(ifs);
	}
	exit(1);
}
void Parser::putFieldIntoRoute(Route &route, const std::string &key,
							   const std::string &value)
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
	else
		exit(1);
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
		unsigned long pos = str.find('#'); /////////////////// Ломается кавычками
		if (pos != std::string::npos)
			str.erase(pos, str.size());
		if (!str.empty())
			break;
	}
	return str;
}

