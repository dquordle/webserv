#ifndef WEB_SERVER_HOST_HPP
#define WEB_SERVER_HOST_HPP

#include <vector>
#include <map>
#include <arpa/inet.h>
#include "Route.hpp"
#include "../Configs/Configuration.hpp"

class Server
{

public:
	Server();

	void 						setIP(const std::string& ip);
	void 						setPort(const std::string& port);
	void 						setServerName(const std::string& name);
	void 						addError(const std::string & error);
//	void 						setMaxBodySize(const std::string & size);
	void 						addRoute(Route & route);
//	void 						setDefault(bool isDef);
	std::string					getIp();
	std::string					getPortStr();
	std::vector<std::string>	getNames();

	Route * chooseRoute(const std::string & target);
//	Route * chooseRoutee(const std::string & target);

	std::string isNonDefaultErrorPage(int statusCode) const;

	std::string					getCGIPath(const std::string & target);


private:
	std::string					_ip;
	std::string					_portStr;
	int 						_port;
	std::vector<std::string>	_server_names;
	std::map<int, std::string>	_errors;
//	int 						_max_body_size;
	std::vector<Route>			_routes;
};

#endif
