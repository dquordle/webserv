#ifndef S_HEADERS_HPP
#define S_HEADERS_HPP

#include <map>
#include <string>
#include <iostream>

struct s_headers {

//    public:
        std::map<std::string, std::string>	headers;
//    private:
	    size_t colon;
	    std::string fieldName;
	    std::string fieldValue;


//	s_headers(const s_headers &other) {
////		*this = other;
//        headers = other.headers;
//	}
//
//	s_headers() {};

//public:
	s_headers &operator=(const s_headers &other) {
        if (this == &other)
            return *this;
        headers = other.headers;
		return *this;
	}

    bool isHostProvided() const {
        if (headers.find("Server") != headers.end())
            return true;
        return false;
    }

    bool isBothContLenAndTransfEncod() {
	    if (headers.find("Content-Length") != headers.end() && headers.find("Transfer-Encoding") != headers.end())
	        return true;
	    return false;
	}

    const std::string getHeaders() const {
        std::map<std::string, std::string>::const_iterator it = headers.begin();
        std::map<std::string, std::string>::const_iterator ite = headers.end();
        std::string ret;

        for (;it != ite; ++it) {
            ret.append(it->first);
            ret.append(": ");
            ret.append(it->second);
            ret.append("\r\n");
        }
        return ret;
    }

    int checkHeaderField(const std::string & field) {
	    if (field.find(' ') == 0) // whitespace-preceded line rfc7230 (3) postman даже не пытается их отправить
	        return 400;
	    colon = field.find(':');
	    if (!isColonCorrect(field))
	        return 400;

	    setFieldName(field);
	    setFieldValue(field);
	    if (fieldName.compare(0, 14, "Content-Length") == 0 && !isContentLengthValid()) // TODO для повторяющихся значений?
	        return 400;
        return 200;
	}

	void setFieldName(const std::string & field) { fieldName = field.substr(0, colon); }

	void setFieldValue(const std::string & field) { fieldValue = field.substr(colon + 1); }

	private:
        bool isColonCorrect(const std::string & field) const {
            if (colon == std::string::npos || colon == 0 || field[colon - 1] == ' ')
                return false;
            return true;
        }

        bool isContentLengthValid() const {
            if (headers.find("Content-Length") != headers.end())
                return false;
            if (fieldValue.find(',') != std::string::npos)
                return false;
            return true;

	}

};

#endif
