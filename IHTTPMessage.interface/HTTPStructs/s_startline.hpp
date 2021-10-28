#ifndef S_STARTLINE_HPP
#define S_STARTLINE_HPP

#include <string>

struct s_startline {
	std::string method;
	std::string target;
	std::string version;

//	s_startline(const s_startline &other) {
//		*this = other;
//	}

	s_startline &operator=(const s_startline &other) {
		this->method = other.method;
		this->target = other.target;
		this->version = other.version;
		return *this;
	}

//	bool isMethodAllowed() const {
//	    if (method == "GET" || method == "POST" || method == "DELETE")
//	        return true;
//	    return false;
//    }

    bool isVersionCorrect() const {
	    if (version.compare("HTTP/1.1" ) == 0)
	        return true;
	    return false;
	}

    bool isMethodImplemented() const {
	    const std::string methods[] = {"GET", "POST", "DELETE", "PUT", "PATCH", "COPY", "HEAD", "OPTIONS",
                                    "LINK", "UNLINK", "PURGE", "LOCK", "UNLOCK", "PROPFIND", "VIEW"};

	    for (int i =0; i < 15 ; ++i)
            if (method.compare(methods[i]) == 0)
                return true;
        return false;
    }
};

#endif
