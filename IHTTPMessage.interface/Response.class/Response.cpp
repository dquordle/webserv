#include "Response.class.hpp"
#include <fstream>

Response::Response(int statusCode, const s_startline &startline, const s_headers &headers, const s_bodies &bodies, Server * server) : _isCGI(false) {
    _s_startline = startline;
    _requestHeaders = headers;
    _s_bodies = bodies;
    _statusCode = statusCode;
	_server = server;
//    TODO: может переенести вставку Referer
_s_startline.target.insert(0, _requestHeaders.getReferer(_s_startline.target));
    setAttributes();
    createResponse();
}

/**
 * fill status line of HTTP Response: protocol version(HTTP/1.1), status code and status text
 */

void Response::makeStartline() {
    _statusLine = "HTTP/1.1 ";

    std::string const arrStatus[] = {"200 OK", "301 Moved Permanently", "400 Bad Request", "403 Forbidden", "404 Not Found", "405 Method Not Allowed",
                                     "501 Not Implemented", "505 HTTP Version Not Supported", "204 No Content", "413 Request Entity Too Large"};
    std::ostringstream ss;
    ss << _statusCode;

    std::string errorNumber = ss.str();

    for (int i = 0; i < 10 ; i++)
        if ((arrStatus[i].find(errorNumber)) != std::string::npos)
        {
            _statusLine.append(arrStatus[i]);
            break;
        }
    _statusLine.append("\r\n");
}

void Response::makeHeaders() {
    setDate();
//    setContentType();
    setContentLength();
    setServerName();
    if (_statusCode == 405)
        _s_headers.headers.insert(std::pair<std::string, std::string>("Allow:", _route->getAllowedMethods()));
    if (_statusCode == 301)
        setLocation();
}

void Response::makeBodies() {
    if (_statusCode == 200)
    {
		_route = _server->chooseRoute(_s_startline.target);
        if (_route == nullptr) {
            _statusCode = 404;
        }
        else {
            if (!_route->isMethodInVector(_s_startline.method)) {
                _statusCode = 405;
            }
            else if (!_route->getRedirection().empty()) {
                _statusCode = 301;
            }
            else if (_route->getMaxBodySize() != 0 && _s_bodies._body.size() > _route->getMaxBodySize())
            	_statusCode = 413;
            checkIfCGI();
        }
    }
    if (_statusCode == 200)
    {
        rewriteTargetIfRoot();
        if (_s_startline.method == "GET")
            doGetMethod();
        else if (_s_startline.method == "POST" || _s_startline.method == "PUT")
            doPostMethod();
        else
            doDeleteMethod();
    }
    if (_statusCode != 200)
        setErrorBody();
}

void Response::rewriteTargetIfRoot() {
    std::string root = _route->getDirectory();

    if (!root.empty()) {
        std::string routeName;

        routeName = _route->getName();
//        if (routeName[routeName.length() - 1] != '/')
            _s_startline.target.erase(0, routeName.length());
        _s_startline.target.insert(0, root);
    }
}

/**
 * calls all setters in order, depending on status code calls either setBody ot setErrorBody
 */

void Response::setAttributes() {
    makeBodies();
    makeStartline();
	makeHeaders();
}

/**
 * set date which will be used as one of the headers in HTTP Response
 */

void Response::setDate() {
    const std::string day[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    const std::string month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    time_t	rawTime;
    struct	tm * timeInfo;
    std::ostringstream out;

    time (&rawTime);
    timeInfo = localtime (&rawTime);

    out << day[timeInfo->tm_wday] << ", "
    << std::setw(2) << std::setfill('0') << timeInfo->tm_mday << " "
    << month[timeInfo->tm_mon] << " " << (timeInfo->tm_year + 1900) << " "
    << std::setw(2) << std::setfill('0') << timeInfo->tm_hour << ":"
    << std::setw(2) << std::setfill('0') << timeInfo->tm_min << ":"
    << std::setw(2) << std::setfill('0') << timeInfo->tm_sec << " GMT";

    _s_headers.headers.insert(std::pair<std::string, std::string>("Date", out.str()));
}

void Response::setServerName() {
    _s_headers.headers.insert(std::pair<std::string, std::string>("Server-Name", "Egor"));
}

void Response::setLocation() {
    _s_headers.headers.insert(std::pair<std::string, std::string>("Location", _route->getRedirection()));
}

/**
 * depending on status code fill body with error message from errorResponse.hpp
 */

void Response::setErrorBody() {
//    TODO: подумать про самопроверку
    std::string err;

    err = _server->isNonDefaultErrorPage(_statusCode);
    if (!err.empty()) {
        err.erase(0,1);

        std::ifstream file(err);
        if (file.is_open()) {
            std::string line;
            while (!file.eof()) {
                getline(file, line);
                _body.append(line);
                if (file.eof())
                    break;
            }
            file.close();
            return;
        }
    }
    setDefaultError();
}

void Response::setDefaultError() {
    switch (_statusCode) {
    	case 204:
    		_body = error_201;
    		break;
        case 301:
            _body = error_301;
            break;
        case 400:
            _body = error_400;
            break;
        case 401:
            _body = error_401;
            break;
        case 403:
            _body = error_403;
            break;
        case 404:
            _body = error_404;
            break;
        case 405:
            _body = error_405;
            break;
        case 413:
            _body = error_413;
            break;
        case 501:
            _body = error_501;
            break;
        case 505:
            _body = error_505;
            break;
    }
}

void Response::doGetMethod() {
    std::string path = _s_startline.target;
    path.erase(0, path.find_first_not_of('/'));

    if (path.empty()) {
        char buf[MAXPATHLEN];
        path = getwd(buf);
    }

    struct stat s;
    if (stat(path.c_str(), &s) == 0) {
        if (s.st_mode & S_IFDIR)
            getFolder(path);
        else if (s.st_mode & S_IFREG)
            getFile(path);
        else
            _statusCode = 403;
    }
    else
        _statusCode = 404;
}

void Response::getFolder(std::string & path) {
    DIR *dir;
    struct dirent *ent;


    if ((dir = opendir(path.c_str())) != NULL) {
        if (_route->isAutoindexOn()) {
            _body = "<html>\n<head><title>Index of ";
            _body.append(_s_startline.target);
            _body.append("</title></head>\n<body bgcolor=\"white\">\n<h1>Index of ");
            _body.append(_s_startline.target);
            _body.append("</h1><hr><pre><a href=\"../\">../</a>\n");
            while ((ent = readdir(dir)) != NULL) {
                if (std::strcmp(ent->d_name, ".") == 0 || std::strcmp(ent->d_name, "..") == 0)
                    continue;
                _body.append("<a href=\"");
                _body.append(ent->d_name);
                if (ent->d_type == DT_DIR)
                    _body.append("/");
                _body.append("\">");
                _body.append(ent->d_name);
                if (ent->d_type == DT_DIR)
                    _body.append("/");
                _body.append("</a>\n");
            }
            _body.append("</pre><hr></body>\n</html>\n");
        } else {
            if (_route->getIndexFile().empty())
                _statusCode = 404;
            else {
                if (path.find(path.length() - 1, '/') == std::string::npos)
                    path.append("/");
                getFile(path.append(_route->getIndexFile()));
            }
//            if (_statusCode == 404)
//                _statusCode = 403;
        }
        closedir (dir);
    } else
        _statusCode = 403; // может заменить на 500 internal server error, a 403 при EACCES
}

void Response::getFile(std::string & path) {
    std::ifstream file(path);

    if (file.is_open()) {
        std::string line;
        while (!file.eof()) {
            getline(file, line);
            _body.append(line);
            if (file.eof())
                break;
        }
        file.close();
    } else
        _statusCode = 404;
}

void Response::doPostMethod() {

    std::string filename = _s_startline.target;
    if (!_isCGI)
	{
		filename.erase(0, filename.find_last_of('/'));
		filename.insert(0, _route->getSavePath());
	}
	filename.erase(0, 1);


    std::ofstream	outfile(filename);
    if (!outfile) {
        _statusCode = 404;
        return ;
    }
	std::string body = _s_bodies._full_request.substr(_s_bodies._full_request.find("\r\n\r\n") + 4);
	if (_isCGI)
	{
//		std::string root = "/YoupiBanane";

//		if (!root.empty()) {
//			std::string routeName;
//
//			routeName = "/directory";
//        if (routeName[routeName.length() - 1] != '/')
//			_s_startline.target.erase(0, routeName.length());
//			_s_startline.target.insert(0, root);
//		}
//		outfile << _s_bodies._full_request;
		outfile << body;
		outfile.close();
		CGI cgi(_cgi_path, _s_startline, _requestHeaders);
		_body = cgi.executeCGI();
	}
	else
	{
		outfile << body;
		outfile.close();
	}
}

void Response::doDeleteMethod() {
    if (_s_startline.target == "/")
        _statusCode = 405;

        std::string filename = _s_startline.target;

    if (remove((filename.erase(0, 1)).c_str()) == 0) {
        _statusCode = 200;
//        _body = "File is deleted";
        return ;
    }
    _statusCode = 404;
}

/**
 * set attribute content length for http header according to rfc7230 3.3.2
 */

void Response::setContentLength() {
    std::ostringstream contentLength;

//    if (_statusCode / 100 == 1 || _statusCode == 204) // такие коды не используются
//        return ;
    contentLength << _body.length();
    _s_headers.headers.insert(std::pair<std::string, std::string>("Content-Length", contentLength.str()));

}

void Response::setContentType() {
    if (_statusCode != 200) {
        _s_headers.headers.insert(std::pair<std::string, std::string>("Content-Type", "text/html"));
        return;
    }

    size_t startOfType = _s_startline.target.find_last_of('.');
    std::string contentType_;

    if (startOfType != std::string::npos)
        contentType_ = _s_startline.target.substr(startOfType + 1, _s_startline.target.length());

    _s_headers.headers.insert(std::pair<std::string, std::string>("Content-Type", contentType_));

}

/**
 * create response appending info from getters in Status-Line -> Headers -> Body order
 */

void Response::createResponse() {
    _response.append(getStatusLine());
    _response.append(_s_headers.getHeaders());
    _response.append("\r\n");
	if (_s_startline.method != "HEAD") ///////////////////////////////////////////////////////////
    	_response.append(getBody());
}

const std::string &Response::getStatusLine() const { return _statusLine; }

const std::string &Response::getBody() const { return _body; }

const std::string &Response::getResponse() const {return _response; }

void Response::checkIfCGI()
{
	_cgi_path = _server->getCGIPath(_s_startline.target);
	if (!_cgi_path.empty())
		_isCGI = true;
}

