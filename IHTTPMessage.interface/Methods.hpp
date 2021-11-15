#ifndef WEB_SERVER_METHODS_HPP
#define WEB_SERVER_METHODS_HPP

#include <iostream>

enum Method
{
	NOT_IMPLEMENTED = -1,
	GET,
	POST,
	DELETE,
	PUT,
	HEAD,
	PATCH,
	COPY,
	OPTIONS,
	LINK,
	UNLINK,
	PURGE,
	LOCK,
	UNLOCK,
	PROPFIND,
	VIEW
};

class Methods
{
public:
	static bool isImplemented(const std::string & methodStr);
	static bool isAllowed(const std::string & methodStr);
	static Method getMethodStoE(const std::string & methodStr);
	static std::string getMethodEtoS(Method & method);
};


#endif
