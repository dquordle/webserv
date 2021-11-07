#ifndef CGI_HPP
#define CGI_HPP

#include "../IHTTPMessage.interface/HTTPStructs/s_headers.hpp"
#include "../IHTTPMessage.interface/HTTPStructs/s_startline.hpp"

class CGI {
    public:
        CGI(std::string path, s_startline &startline, s_headers &headers);
        void executeCGI();
        ~CGI();

    private:
        s_headers _requestHeaders;
        s_startline _requestStartLine;
        std::string _path;
        std::map<std::string, std::string> _envp;

        void setEnv();
};

#endif
