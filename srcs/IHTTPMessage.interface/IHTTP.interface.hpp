#ifndef IHTTP_INTERFACE_HPP
#define IHTTP_INTERFACE_HPP

#include "HTTPStructs/s_startline.hpp"
#include "HTTPStructs/s_headers.hpp"
#include "HTTPStructs/s_bodies.hpp"

class IHTTPMessage {
protected:
	s_startline _s_startline;
	s_headers	_s_headers;
	s_bodies	_s_bodies;
	int         _statusCode;
public:
	virtual void makeStartline() = 0;
	virtual void makeHeaders() = 0;
	virtual void makeBodies() = 0;
};

#endif
