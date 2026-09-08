#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>

class Response {
public:
    std::string build(int status, const std::string& body, const std::string& content_type);
    std::string buildRedirect(int status, const std::string& location);
};


std::string buildBody(int code, const std::string& message);
std::string statusText(int code);

#endif