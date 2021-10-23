#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include "../IHTTP.interface.hpp"

class Request : IHTTPMessage {
private:
	const std::string request_;
	bool isBodies_;

public:
	explicit Request(const std::string &request);

	const s_startline &getStartLine() const;

	const s_headers &getHeaders() const;

	const s_bodies &getBodies() const;

	int getStatusCode() const { return statusCode_; }

private:
	virtual void makeStartline();
	virtual void makeHeaders();
	virtual void makeBodies();

private:
	std::vector<std::string>	splitVector(std::string lines, const std::string &delimiter = "\n", bool keepDelimiter = true) const;
//    int statusCode_;
};

#endif
