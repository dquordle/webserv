#include "cgi.hpp"

CGI::CGI(const std::string& path, s_startline &startline, s_headers &headers) : _requestHeaders(headers), _requestStartLine(startline),
                                                                         _path(path) {
    setEnv();
}

CGI::~CGI() {}

std::string	CGI::executeCGI()
{
	std::stringstream cgi;
	char buf[MAXPATHLEN];
	std::string path = getwd(buf) + _requestStartLine.target;
	cgi << _path << " < " <<  path << " > " << CGI_OUTPUT_FILE;
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
	unsetEnv();
	return body;
}

void CGI::setEnv() {
    _envp["SERVER_SOFTWARE"] = "Egor/1.0";
    _envp["SERVER_NAME"] = _requestHeaders.getHost().substr(0, _requestHeaders.getHost().find(':'));
    _envp["GATEWAY_INTERFACE"] = "CGI/1.0";
    _envp["SERVER_PROTOCOL"] = "HTTP/1.1";
    _envp["SERVER_PORT"] = _requestHeaders.getHost().substr(_requestHeaders.getHost().find(':') + 1);
    _envp["REQUEST_METHOD"] = _requestStartLine.method;
    _envp["PATH_INFO"] = _path;

    _envp["QUERY_STRING"] = _requestStartLine.target.substr(_requestStartLine.target.find('?') + 1);
    if (_requestStartLine.method == "POST")
        _envp["CONTENT_TYPE"] = _requestHeaders.getContentType();
    if (!_requestHeaders.getContentLength().empty())
        _envp["CONTENT_LENGTH"] = _requestHeaders.getContentLength();



	std::map<std::string, std::string>::iterator it = _requestHeaders.headers.begin();
	for (; it != _requestHeaders.headers.end(); it++)
	{
		std::string envName = getEnvName(it->first);
		envName.insert(0, "HTTP_");
		_envp[envName] = it->second;
	}

	it = _envp.begin();
	for (; it != _envp.end(); it++)
	{
		std::string env = it->first + "=" + it->second;
		char cstr[env.length() + 1];
		strcpy(cstr, env.c_str());
		setenv(it->first.c_str(), it->second.c_str(), 1);
	}
}

void CGI::unsetEnv()
{
	std::map<std::string, std::string>::iterator it = _envp.begin();
	for (; it != _envp.end(); it++)
		unsetenv(it->first.c_str());
}

std::string CGI::getEnvName(std::string headerName)
{
	const char *arr = headerName.c_str();
	std::string res;
	for (size_t i = 0; i < headerName.size(); i++)
	{
		if (arr[i] == '-')
			res.push_back('_');
		else
			res.push_back(std::toupper(arr[i]));
	}
	return res;
}
