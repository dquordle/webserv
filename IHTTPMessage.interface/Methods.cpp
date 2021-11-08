#include "Methods.hpp"

bool Methods::isImplemented(const std::string & methodStr)
{
	Method method = getMethodStoE(methodStr);
	if (method == NOT_IMPLEMENTED)
		return false;
	return true;
}

bool Methods::isAllowed(const std::string & methodStr)
{
	Method method = getMethodStoE(methodStr);
	if (method != NOT_IMPLEMENTED && method < 4)
		return true;
	return false;
}

std::string Methods::getMethodEtoS(Method &method)
{
	const std::string methodsArray[] = {"GET", "POST", "DELETE", "PUT", "PATCH", "COPY", "HEAD", "OPTIONS",
										"LINK", "UNLINK", "PURGE", "LOCK", "UNLOCK", "PROPFIND", "VIEW"};
	return methodsArray[method];
}

Method Methods::getMethodStoE(const std::string & methodStr)
{
	const std::string methodsArray[] = {"GET", "POST", "DELETE", "PUT", "PATCH", "COPY", "HEAD", "OPTIONS",
								   "LINK", "UNLINK", "PURGE", "LOCK", "UNLOCK", "PROPFIND", "VIEW"};
	for (int i = 0; i < 15 ; ++i)
	{
		if (methodStr == methodsArray[i])
			return Method(i);
	}
	return NOT_IMPLEMENTED;
}
