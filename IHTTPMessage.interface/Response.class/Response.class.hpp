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

class Response : public IHTTPMessage {
private:
    s_headers   requestHeaders_;
    std::string response_;
    std::string statusLine_;
    std::string body_;

public:
    Response(int statusCode_, const s_startline &startline, const s_headers &headers, const s_bodies &bodies);

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
    void setErrorBody();
    void getFolder(const std::string & );
    void getFile(const std::string & );
    void doGetMethod();
    void doPostMethod();
    void doDeleteMethod();

    const std::string &getStatusLine() const;
    const std::string &getBody() const;

};

#endif
