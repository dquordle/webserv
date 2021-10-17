
#ifndef ERRORRESPONSE_HPP
#define ERRORRESPONSE_HPP
# include <string>

static const std::string error_200 =
        "<html>\r\n"
        "<head><title>200 OK</title></head>\r\n"
        "<body bgcolor=\"white\">\r\n"
        "<center><h1>200 OK</h1></center>\r\n";

static const std::string error_400 =
        "<html>\r\n"
        "<head><title>400 Bad Request</title></head>\r\n"
        "<body bgcolor=\"white\">\r\n"
        "<center><h1>400 Bad Request</h1></center>\r\n";

static const std::string error_401 =
        "<html>\r\n"
        "<head><title>401 Authorization Required</title></head>\r\n"
        "<body bgcolor=\"white\">\r\n"
        "<center><h1>401 Authorization Required</h1></center>\r\n";

static const std::string error_403 =
        "<html>\r\n"
        "<head><title>403 Forbidden</title></head>\r\n"
        "<body bgcolor=\"white\">\r\n"
        "<center><h1>403 Forbidden</h1></center>\r\n";

static const std::string error_404 =
        "<html>\r\n"
        "<head><title>404 Not Found</title></head>\r\n"
        "<body bgcolor=\"white\">\r\n"
        "<center><h1>404 Not Found</h1></center>\r\n";

static const std::string error_405 =
        "<html>\r\n"
        "<head><title>405 Method Not Allowed</title></head>\r\n"
        "<body bgcolor=\"white\">\r\n"
        "<center><h1>405 Method Not Allowed</h1></center>\r\n";

static const std::string error_418 =
        "<html>\r\n"
        "<head><title>418 I'm a teapot</title></head>\r\n"
        "<body bgcolor=\"white\">\r\n"
        "<center><h1>418 I'm a teapot</h1></center>\r\n";

static const std::string error_501 =
        "<html>\r\n"
        "<head><title>501 Not Implemented</title></head>\r\n"
        "<body bgcolor=\"white\">\r\n"
        "<center><h1>501 Not Implemented</h1></center>\r\n";

static const std::string error_505 =
        "<html>\r\n"
        "<head><title>505 HTTP Version Not Supported</title></head>\r\n"
        "<body bgcolor=\"white\">\r\n"
        "<center><h1>505 HTTP Version Not Supported</h1></center>\r\n";


#endif
