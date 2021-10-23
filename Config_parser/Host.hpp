#ifndef WEB_SERVER_HOST_HPP
#define WEB_SERVER_HOST_HPP

#include <vector>
#include <map>
#include "Route.hpp"


class Host
{


public:
	Host();
	~Host();

	void setIP(const std::string& ip);
	void setPort(const std::string& port);
	void setServerName(const std::string& name);
	void addError(const std::string & error);
	void setMaxBodySize(const std::string & size);
	void addRoute(Route & route);
	void setAddress();
	bool addAddress();
	void setDefault(bool isDef);

	bool isDefault() const;
	int	createSocket();




private:
	std::string					_ip;
	std::string					_portStr;
	int 						_port;
	std::vector<std::string>	_server_name;
	bool						_is_default;
	std::map<int, std::string>	_errors;
	int 						_max_body_size;
	std::vector<Route>			_routes;

	std::string					_address;

public:
	static std::vector<std::string> addresses;
};


#endif
