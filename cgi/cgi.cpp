//
// Created by Bumblebee Goat on 11/4/21.
//

#include "cgi.hpp"

CGI::CGI(const std::string& path, s_startline &startline, s_headers &headers) : _path(path), _requestStartLine(startline),
                                                                         _requestHeaders(headers) {
    setEnv();
}

CGI::~CGI() {}

std::string	CGI::executeCGI()
{
	std::stringstream cgi;
	char buf[MAXPATHLEN];
	std::string path = getwd(buf) + _requestStartLine.target;
	cgi << _path << " < " <<  path << " > " << CGI_OUTPUT_FILE;
	char* meth = getenv("REQUEST_METHOD");
	system(cgi.str().c_str());
	std::ifstream file(CGI_OUTPUT_FILE);
	if (!file.is_open())
	{
		Debug::Log("Error occurred opening CGI result file", true);
		return "";
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	unsigned long start = buffer.str().find("\r\n\r\n") + 4;
	std::string body = buffer.str().substr(start);
	return body;
	/////// Из файла убрать хедер, дописать свой и отправить это добро ответом;
}

void CGI::setEnv() {
    _envp["SERVER_SOFTWARE"] = "Egor/1.0"; // name/version of HTTP server.
    _envp["SERVER_NAME"] = _requestHeaders.getHost().substr(0, _requestHeaders.getHost().find(':')); // host name of the server, may be dot-decimal IP address
    _envp["GATEWAY_INTERFACE"] = "CGI/1.0"; // CGI/version

    _envp["SERVER_PROTOCOL"] = "HTTP/1.1"; //  HTTP/version
    _envp["SERVER_PORT"] = _requestHeaders.getHost().substr(_requestHeaders.getHost().find(':') + 1); // TCP port (decimal)
    _envp["REQUEST_METHOD"] = _requestStartLine.method; // name of HTTP method (see above)
//    _envp["PATH_INFO"] = ; // path suffix, if appended to URL after program name and a slash
//    if (_envp.find("PATH_INFO") != _envp.end())
//        _envp["PATH_TRANSLATED"] = ; // corresponding full path as supposed by server, if PATH_INFO is present
//
//    _envp["SCRIPT_NAME"] = _path; // relative path to the program, like /cgi-bin/script.cgi.
    _envp["PATH_INFO"] = _path; // relative path to the program, like /cgi-bin/script.cgi.

    _envp["QUERY_STRING"] = _requestStartLine.target.substr(_requestStartLine.target.find('?') + 1); // the part of URL after ? character. The query string may be composed of *name=value pairs separated with ampersands (such as var1=val1&var2=val2...) when used to submit form data transferred via GET method as defined by HTML application/x-www-form-urlencoded.
//    _envp["REMOTE_HOST"] = ; // host name of the client, unset if server did not perform such lookup.
//    _envp["REMOTE_ADDR"] = ; // IP address of the client (dot-decimal).
//    _envp["AUTH_TYPE"] = ; // identification type, if applicable.
//    _envp["REMOTE_USER"] = ; //used for certain AUTH_TYPEs.
//    _envp["REMOTE_IDENT"] = ; // see ident, only if server performed such lookup.
    if (_requestStartLine.method == "POST")
        _envp["CONTENT_TYPE"] = _requestHeaders.getContentType(); // Internet media type of input data if PUT or POST method are used, as provided via HTTP header.
    if (!_requestHeaders.getContentLength().empty())
        _envp["CONTENT_LENGTH"] = _requestHeaders.getContentLength(); // similarly, size of input data (decimal, in octets) if provided via HTTP header.

	std::map<std::string, std::string>::iterator it = _envp.begin();
	for (; it != _envp.end(); it++)
	{
		std::string env = it->first + "=" + it->second;
		char cstr[env.length() + 1];
		strcpy(cstr, env.c_str());
		int num = setenv(it->first.c_str(), it->second.c_str(), 1);
		char* meth = getenv(it->first.c_str());
		num = 36 + 2;
	}
	char* meth = getenv("REQUEST_METHOD");
//            Variables passed by user agent (HTTP_ACCEPT, HTTP_ACCEPT_LANGUAGE, HTTP_USER_AGENT, HTTP_COOKIE and possibly others) contain values of corresponding HTTP headers and therefore have the same sense.
}