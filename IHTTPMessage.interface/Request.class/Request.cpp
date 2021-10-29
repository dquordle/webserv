#include "Request.hpp"

Request::Request(const std::string &request) : request_(request) {
    makeStartline();
    isBodies_ = (request_.find("\r\n\r\n") + 4 != request_.length());
    if (statusCode_ == 200)
        makeHeaders();
    if (statusCode_ == 200)
        makeBodies();
}

const s_startline &Request::getStartLine() const {
	return this->s_startline_;
}

const s_headers &Request::getHeaders() const {
	return this->s_headers_;
}

const s_bodies &Request::getBodies() const {
	return this->s_bodies_;
}

void Request::makeStartline() {
//    size_t begin = 0;
//    if (request_.compare(0, 2, "\r\n") == 0)
//        begin = 2;
//    std::string	startline = request_.substr(begin, request_.find('\r'));

	std::string	startline = request_.substr(0, request_.find('\r'));

	size_t endMethod, endTarget;
	endMethod = startline.find(' ');
	endTarget = startline.find_last_of(' ');

	if (endMethod == std::string::npos || endTarget == std::string::npos ||
	endMethod == endTarget) {
	    statusCode_ = 400;
        return;
	}

	this->s_startline_.method = startline.substr(0, endMethod);

    if (!s_startline_.isMethodImplemented()) {
        statusCode_ = 501;
        return ;
    }

	if (!s_startline_.isMethodAllowed()) {
	    statusCode_ = 405;
	    return ;
	}

	startline.erase(0, endMethod + 1);

	this->s_startline_.target = startline.substr(0, endTarget - endMethod - 1);
	this->s_startline_.version = startline.substr(startline.find_last_of(' ') + 1);

	if (s_startline_.version.find("HTTP") != 0) {
	    statusCode_ = 400;
	    return;
	}

	if (!s_startline_.isVersionCorrect()) {
	    statusCode_ = 505;
	    return ;
	}
	statusCode_ = 200;
}

/*
 * fill attribute s_headers_ (map) with key and value from request_ string
 *
 * checks if there whitespace between the header field-name and colon or multiple header fields (according to rfc7230 3.2.4)
 * and sets statusCode__ to 400 in that case
 *
 * at the end of function checks if Server field-name is provided (according to rfc7230 5.4),
 * sets 400 in statusCode__ otherwise
 */

void Request::makeHeaders() {
    std::vector<std::string> _vecHeaders;

    size_t headersBegin = request_.find("\r\n") + 2;
    size_t headersLength = request_.find("\r\n\r\n") - headersBegin + 2;
    _vecHeaders = splitVector(request_.substr(headersBegin, headersLength), "\r\n", false);

    std::vector<std::string>::const_iterator it = _vecHeaders.cbegin();
    std::vector<std::string>::const_iterator ite = _vecHeaders.cend();
    for (;it < ite; it++) {
        if ((statusCode_ = s_headers_.checkHeaderField(*it)) != 200) {
            s_headers_.headers.clear();
            return;
        }
        s_headers_.headers[s_headers_.fieldName] = s_headers_.fieldValue;
    }
    if (!s_headers_.isHostProvided() || s_headers_.isBothContLenAndTransfEncod())
        statusCode_ = 400;
//    s_headers_.print();
}

void Request::makeBodies()  {
	if (!isBodies_)
		return;

	std::string bodyBegin = request_.substr(request_.find("\r\n\r\n") + 4);
    std::vector<std::string> vSplit = splitVector(bodyBegin);

    s_bodies_.bodies = vSplit;
    if (this->s_bodies_.bodies.empty())
		this->s_bodies_.bodies.clear();
}

std::vector<std::string> Request::splitVector(std::string lines, const std::string &delimiter, bool keepDelimiter) const  {
	std::vector<std::string>	result;
	std::string token;
	size_t lengthToAdd = (keepDelimiter ? delimiter.length() : 0);
	size_t pos = lines.find(delimiter);

	for (; pos != std::string::npos; pos = lines.find(delimiter)) {
		token = lines.substr(0, pos + lengthToAdd);
		result.push_back(token);
		lines.erase(0, pos + delimiter.length());
	}
	if (isBodies_ && keepDelimiter)
		result.push_back(lines.substr(0, pos));
	return result;
}
