#include "Request.hpp"

Request::Request(const std::string &request) : _request(request) {
    makeStartline();
    _isBody = (_request.find("\r\n\r\n") + 4 != _request.length());
    if (_statusCode == 200)
        makeHeaders();
    if (_statusCode == 200)
        makeBodies();
}

const s_startline &Request::getStartLine() const {
	return this->_s_startline;
}

const s_headers &Request::getHeaders() const {
	return this->_s_headers;
}

const s_bodies &Request::getBodies() const {
	return this->_s_bodies;
}

void Request::makeStartline() {
//    size_t begin = 0;
//    if (_request.compare(0, 2, "\r\n") == 0)
//        begin = 2;
//    std::string	startline = _request.substr(begin, _request.find('\r'));

	std::string	startline = _request.substr(0, _request.find('\r'));

	size_t endMethod, endTarget;
	endMethod = startline.find(' ');
	endTarget = startline.find_last_of(' ');

	if (endMethod == std::string::npos || endTarget == std::string::npos ||
	endMethod == endTarget) {
	    _statusCode = 400;
        return;
	}

	this->_s_startline.method = startline.substr(0, endMethod);

    if (!_s_startline.isMethodImplemented()) {
        _statusCode = 501;
        return ;
    }

	startline.erase(0, endMethod + 1);

	this->_s_startline.target = startline.substr(0, endTarget - endMethod - 1);
	this->_s_startline.version = startline.substr(startline.find_last_of(' ') + 1);

	if (_s_startline.version.find("HTTP") != 0) {
	    _statusCode = 400;
	    return;
	}

	if (!_s_startline.isVersionCorrect()) {
	    _statusCode = 505;
	    return ;
	}
	_statusCode = 200;
}

/*
 * fill attribute _s_headers (map) with key and value from _request string
 *
 * checks if there whitespace between the header field-name and colon or multiple header fields (according to rfc7230 3.2.4)
 * and sets __statusCode to 400 in that case
 *
 * at the end of function checks if Server field-name is provided (according to rfc7230 5.4),
 * sets 400 in __statusCode otherwise
 */

void Request::makeHeaders() {
    std::vector<std::string> _vecHeaders;

    size_t headersBegin = _request.find("\r\n") + 2;
    size_t headersLength = _request.find("\r\n\r\n") - headersBegin + 2;
    _vecHeaders = splitVector(_request.substr(headersBegin, headersLength), "\r\n", false);

    std::vector<std::string>::const_iterator it = _vecHeaders.cbegin();
    std::vector<std::string>::const_iterator ite = _vecHeaders.cend();
    for (;it < ite; it++) {
        if ((_statusCode = _s_headers.checkHeaderField(*it)) != 200) {
            _s_headers.headers.clear();
            return;
        }
        _s_headers.headers[_s_headers.fieldName] = _s_headers.fieldValue;
    }
    if (!_s_headers.isHostProvided() || _s_headers.isBothContLenAndTransfEncod())
        _statusCode = 400;
//    _s_headers.print();
}

void Request::makeBodies()  {
	if (!_isBody)
		return;

	std::string bodyBegin = _request.substr(_request.find("\r\n\r\n") + 4);
	//	TODO: isBodyLimit - проверить что не был ограничен размер на максимальный размер запроса, если размер тела превышает его то - HTTP 413 (Request Entity Too Large Error)
    std::vector<std::string> vSplit = splitVector(bodyBegin);

    _s_bodies.bodies = vSplit;
    if (this->_s_bodies.bodies.empty()) ///wat?
		this->_s_bodies.bodies.clear();
}

int Request::getStatusCode() const { return _statusCode; }

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
	if (_isBody && keepDelimiter)
		result.push_back(lines.substr(0, pos));
	return result;
}
