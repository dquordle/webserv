#ifndef RESPONSE_CLASS_HPP
#define RESPONSE_CLASS_HPP

#include "../IHTTP.interface.hpp"
#include "errorResponse.hpp"
#include <sstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <dirent.h>
#include <cstring>
#include "../../Config_parser/Host.hpp"

class Response : public IHTTPMessage {
private:
    s_headers   _requestHeaders;
    std::string _response;
    std::string _statusLine;
    std::string _body;
    Host *      _host;
    Route *     _route;

public:
    Response(int statusCode, const s_startline &startline, const s_headers &headers, const s_bodies &bodies, Host *);

const std::string &getResponse() const;

private:
    virtual void makeStartline();
    virtual void makeHeaders();
    virtual void makeBodies();

    void createResponse();

    void setAttributes();
    void setDate();
    void setContentLength();
    void setContentType();
    void setServerName();

    void setDefaultError();
    void setErrorBody();

    void getFolder(std::string & );
    void getFile(std::string & );
    void doGetMethod();

    void doPostMethod();
    void doDeleteMethod();

    const std::string &getStatusLine() const;
    const std::string &getBody() const;

};

#endif
