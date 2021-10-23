#include "Response.class.hpp"
#include <fstream>

Response::Response(int statusCode, const s_startline &startline, const s_headers &headers, const s_bodies &bodies) {
    s_startline_ = startline;
    requestHeaders_ = headers;
    s_bodies_ = bodies;
    statusCode_ = statusCode;
    setAttributes();
    createResponse();
}

/**
 * fill status line of HTTP Response: protocol version(HTTP/1.1), status code and status text
 */

void Response::makeStartline() {
    statusLine_ = "HTTP/1.1 ";

    std::string const arrStatus[] = {"200 OK", "400 Bad Request", "404 Not Found", "405 Method Not Allowed",
                                     "501 Not Implemented", "505 HTTP Version Not Supported"};
    std::ostringstream ss;
    ss << statusCode_;

    std::string errorNumber = ss.str();

    for (int i = 0; i < 5 ; i++)
        if ((arrStatus[i].find(errorNumber)) != std::string::npos)
        {
            statusLine_.append(arrStatus[i]);
            break;
        }
    statusLine_.append("\r\n");
}

void Response::makeHeaders() {
    setDate();
//    setContentType();
    setContentLength();
//    if (statusCode_ == 405)
//        s_headers_.headers.insert(std::pair<std::string, std::string>("Allow:","GET, POST, DELETE\r\n"));
}

void Response::makeBodies() {
    if (statusCode_ == 200)
    {
        if (s_startline_.method == "GET")
            doGetMethod();
        else if (s_startline_.method == "POST")
            doPostMethod();
        else
            doDeleteMethod();
    }
    if (statusCode_ != 200)
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
    const std::string day[]={"Sun","Mon","Tue", "Wed","Thu","Fri","Sat"};

    const std::string month[]={"Jan","Feb","Mar", "Apr","May","Jun","Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

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
    out.str();

    s_headers_.headers.insert(std::pair<std::string, std::string>("Date", out.str()));
}

/**
 * depending on status code fill body with error message from errorResponse.hpp
 */

void Response::setErrorBody() {
    switch (statusCode_) {
        case 400:
            body_ = error_400;
            break;
        case 401:
            body_ = error_401;
            break;
        case 403:
            body_ = error_403;
            break;
        case 404:
            body_ = error_404;
            break;
        case 405:
            body_ = error_405;
            break;
        case 418:
            body_ = error_418;
            break;
        case 501:
            body_ = error_501;
            break;
        case 505:
            body_ = error_505;
            break;
    }
}

void Response::doGetMethod() {
//    TODO: добавить автоиндекс и подстроить код под него
    //    if (s_startline_.target == "/")
//        return ;

    std::string filename = s_startline_.target;
    if (filename[0] == '/')
        filename.erase(0, 1);

    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        while (!file.eof()) {
            getline(file, line);
            body_.append(line);
            if (file.eof())
                break ;
        }
        file.close();
    }
    else
        statusCode_ = 404;
}

void Response::doPostMethod() {

    std::string filename = s_startline_.target;
    filename.erase(0, 1);

    std::ofstream	outfile(filename);
    if (!outfile) {
        statusCode_ = 404;
        return ;
    }

    std::vector<std::string>::iterator it = s_bodies_.bodies.begin();
    std::vector<std::string>::iterator ite = s_bodies_.bodies.end();

    for (; it != ite; ++it) {
        outfile << (*it);
    }
    outfile.close();
}

void Response::doDeleteMethod() {
//    if (s_startline_.target == "/") {
//        statusCode_ = 405;
//        s_headers_.headers.insert(std::pair<std::string, std::string>("Allow:","GET\r\n"));
//    }
}

/**
 * set attribute content length for http header according to rfc7230 3.3.2
 */

void Response::setContentLength() {
    std::ostringstream contentLength;

//    if (statusCode_ / 100 == 1 || statusCode_ == 204) // такие коды не используются
//        return ;
    contentLength << body_.length();
    s_headers_.headers.insert(std::pair<std::string, std::string>("Content-Length", contentLength.str()));

}

void Response::setContentType() {
    if (statusCode_ != 200) {
        s_headers_.headers.insert(std::pair<std::string, std::string>("Content-Type", "text/html"));
        return;
    }

    size_t startOfType = s_startline_.target.find_last_of('.');
    std::string contentType_;

    if (startOfType != std::string::npos)
        contentType_ = s_startline_.target.substr(startOfType + 1, s_startline_.target.length());

    s_headers_.headers.insert(std::pair<std::string, std::string>("Content-Type", contentType_));

}

/**
 * create response appending info from getters in Status-Line -> Headers -> Body order
 */

void Response::createResponse() {
    response_.append(getStatusLine());
    response_.append(s_headers_.getHeaders());
    response_.append("\r\n");
    response_.append(getBody());
}

const std::string &Response::getStatusLine() const { return statusLine_; }

const std::string &Response::getBody() const { return body_; }

const std::string &Response::getResponse() const {return response_; }
