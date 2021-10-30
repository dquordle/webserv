#include "Response.class.hpp"
#include <fstream>

Response::Response(int statusCode, const s_startline &startline, const s_headers &headers, const s_bodies &bodies, Host * host) {
    _s_startline = startline;
    _requestHeaders = headers;
    _s_bodies = bodies;
    _statusCode = statusCode;
    _host = host;
//    TODO: может переенести вставку Referer
    _s_startline.target.insert(0, _requestHeaders.getReferer());
    setAttributes();
    createResponse();
}

/**
 * fill status line of HTTP Response: protocol version(HTTP/1.1), status code and status text
 */

void Response::makeStartline() {
    _statusLine = "HTTP/1.1 ";

    std::string const arrStatus[] = {"200 OK", "400 Bad Request", "403 Forbidden", "404 Not Found", "405 Method Not Allowed",
                                     "501 Not Implemented", "505 HTTP Version Not Supported"};
    std::ostringstream ss;
    ss << _statusCode;

    std::string errorNumber = ss.str();

    for (int i = 0; i < 5 ; i++)
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
//    if (_statusCode == 405)
//        _s_headers.headers.insert(std::pair<std::string, std::string>("Allow:","GET, POST, DELETE\r\n"));
}

Route * Host::chooseRoute(const std::string & target) {
    std::vector<Route>::iterator it = _routes.begin();
    std::vector<Route>::iterator ite = _routes.end();
    int maxdepth = -1;
    int currentdepth = -1;
    Route ref = *it;

    for (; it != ite; ++it) {
        if (Route::findTarget((*it), target)) {
            currentdepth = Route::nameDepth(*it);
            if (currentdepth > maxdepth)
            {
                maxdepth = currentdepth;
                ref = *it;
            }
        }
    }
    if (maxdepth == -1)
        return nullptr;
    return new Route(ref);
}

void Response::makeBodies() {
    if (_statusCode == 200)
    {
        _route = _host->chooseRoute(_s_startline.target);
        if (_route == nullptr) {
            _statusCode = 404;
            setErrorBody();
            return;
        }
        if (_s_startline.method == "GET")
            doGetMethod();
        else if (_s_startline.method == "POST")
            doPostMethod();
        else
            doDeleteMethod();
    }
    if (_statusCode != 200)
        setErrorBody();
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
//    TODO: think about normal name
// if getServerName from host (aka server) do insert else do not
    _s_headers.headers.insert(std::pair<std::string, std::string>("Server-Name", "╮(￣_￣)╭"));
}

/**
 * depending on status code fill body with error message from errorResponse.hpp
 */

void Response::setErrorBody() {
//    TODO: подумать про самопроверку
    std::string err;

    err = _host->isNonDefaultErrorPage(_statusCode);
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
        case 418:
            _body = error_418;
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
//TODO: переписать target если directory
// моежт не надо создавать path а просто переписать target

    path.erase(0, 1);
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
            _body = "<html>\n<head><title>Index of /</title></head>\n<body bgcolor=\"white\">\n<h1>Index of /</h1><hr><pre><a href=\"../\">../</a>\n";
            while ((ent = readdir(dir)) != NULL) {
                if (std::strcmp(ent->d_name, ".") == 0 || std::strcmp(ent->d_name, "..") == 0)
                    continue;
                _body.append("<a href=\"");
                _body.append(ent->d_name);
                _body.append("\">");
                _body.append(ent->d_name);
                _body.append("</a>\n");
            }
            _body.append("</pre><hr></body>\n</html>\n");
        } else {
            getFile(path.append("/index.html"));
            if (_statusCode == 404)
                _statusCode = 403;
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
    filename.erase(0, filename.find_last_of('/'));
    filename.insert(0, _route->getSavePath());
    filename.erase(0, 1);

    std::ofstream	outfile(filename);
    if (!outfile) {
        _statusCode = 404;
        return ;
    }

    std::vector<std::string>::iterator it = _s_bodies.bodies.begin();
    std::vector<std::string>::iterator ite = _s_bodies.bodies.end();

    for (; it != ite; ++it) {
        outfile << (*it);
    }
    outfile.close();
}

void Response::doDeleteMethod() {
//    TODO: decide how delete works with folders
//    if (_s_startline.target == "/") {
//        _statusCode = 405;
//        _s_headers.headers.insert(std::pair<std::string, std::string>("Allow:","GET\r\n"));
//    }
    std::string filename = _s_startline.target;

    if (remove((filename.erase(0, 1)).c_str()) == 0) {
        _statusCode = 200;
        _body = "File is deleted";
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
    _response.append(getBody());
}

const std::string &Response::getStatusLine() const { return _statusLine; }

const std::string &Response::getBody() const { return _body; }

const std::string &Response::getResponse() const {return _response; }
