#ifndef WEB_SERVER_HOST_HPP
#define WEB_SERVER_HOST_HPP

#include <vector>
#include <map>
#include "Route.hpp"


class Host
{


public:
//	typedef struct	errors
//	{
//		std::vector<int>	codes;
//		std::string			page;
//		errors				*next;
//		errors(std::vector<int>	& codes, std::string	& page);
//	} 				structErrors;

	Host();
//	Host(const Host & host);
	~Host();

	void setIP(const std::string& ip);
	void setPort(const std::string& port);
	void setServerName(const std::string& name);
	void addError(const std::string & error);
	void setMaxBodySize(const std::string & size);
	void addRoute(Route & route);

	Route * chooseRoute(const std::string & target);
	void finishInitialization();



private:
	std::string					_ip;
	std::string					_portStr;
	int 						_port;
	std::vector<std::string>	_server_name;
	bool						_is_default;
//	structErrors		*_errors;
	std::map<int, std::string>	_errors;
	int 						_max_body_size;
	std::vector<Route>			_routes;


	static std::vector<std::string> _addresses;
};


#endif
