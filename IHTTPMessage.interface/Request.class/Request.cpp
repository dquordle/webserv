#include "Request.hpp"

Request::Request(const std::string &request) : request_(request), responseType(0) {
    makeStartline();
    isBodies_ = (request_.find("\r\n\r\n") + 4 != request_.length());
    if (responseType == 200)
        makeHeaders();
    if (responseType == 200)
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
	std::string	startline = request_.substr(0, request_.find('\r'));

	size_t endMethod, endTarget;
	endMethod = startline.find(' ');
	endTarget = startline.find_last_of(' ');

	if (endMethod == std::string::npos || endTarget == std::string::npos ||
	endMethod == endTarget) {
	    responseType = 400;
        return;
	}

	this->s_startline_.method = startline.substr(0, endMethod);
    if (!s_startline_.isMethodImplemented()) {
        responseType = 501;
        return ;
    }

	if (!s_startline_.isMethodAllowed()) {
	    responseType = 405;
	    return ;
	}

	startline.erase(0, endMethod + 1);

	this->s_startline_.target = startline.substr(0, endTarget - endMethod - 1);
	this->s_startline_.version = startline.substr(startline.find_last_of(' ') + 1);

	if (s_startline_.version.find("HTTP") != 0) {
	    responseType = 400;
	    return;
	}

	if (!s_startline_.isVersionCorrect()) {
	    responseType = 505;
	    return ;
	}
	responseType = 200;
}

/*
 * fill attribute s_headers_ (map) with key and value from request_ string
 *
 * checks if there whitespace between the header field-name and colon or multiple header fields (according to rfc7230 3.2.4)
 * and sets responseType_ to 400 in that case
 *
 * at the end of function checks if Host field-name is provided (according to rfc7230 5.4),
 * sets 400 in responseType_ otherwise
 */

void Request::makeHeaders() {
    std::vector<std::string> _vecHeaders;

    size_t headersBegin = request_.find("\r\n") + 2;
    size_t headersLength = request_.find("\r\n\r\n") - headersBegin + 2;
    _vecHeaders = splitVector(request_.substr(headersBegin, headersLength), "\r\n", false);
    size_t colon;

    std::vector<std::string>::const_iterator it = _vecHeaders.cbegin();
    std::vector<std::string>::const_iterator ite = _vecHeaders.cend();
    for (;it < ite; it++) {
        colon = (*it).find(':');
        if (colon == std::string::npos || colon == 0 || (*it)[colon - 1] == ' ') { //rfc 7230 (3.2.4)
            responseType = 400;
        }
        this->s_headers_.headers[(*it).substr(0, colon)] = (*it).substr(colon + 1);
    }
    if (!s_headers_.isHostProvided())
        responseType = 400;
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
