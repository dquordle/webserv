#ifndef CGI_HPP
#define CGI_HPP

#include <sstream>
#include <fstream>
#include <sys/param.h>
#include <vector>
#include "../IHTTPMessage.interface/HTTPStructs/s_headers.hpp"
#include "../IHTTPMessage.interface/HTTPStructs/s_startline.hpp"
#include "../Configs/Configuration.hpp"
#include "../Debug.class/Debug.hpp"

class CGI {
    public:
        CGI(const std::string& path, s_startline &startline, s_headers &headers);
        std::string executeCGI();
        ~CGI();

    private:
        s_headers _requestHeaders;
        s_startline _requestStartLine;
        std::string _path;
        std::map<std::string, std::string> _envp;
        std::vector<std::string>		headersToSkip;

        void setEnv();
        void unsetEnv();
        std::string	getEnvName(std::string headerName);
//        void fillHeadersVector();
};

#endif
