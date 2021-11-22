#ifndef SERVEREXCEPTION_HPP
#define SERVEREXCEPTION_HPP

#include <exception>
#include <string>

class ServerException : public std::exception {

public:
	virtual const char* what() const throw();
	explicit ServerException(const std::string &);
	~ServerException() _NOEXCEPT;
private:
	std::string logLine_;
};

#endif
