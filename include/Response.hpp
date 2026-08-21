#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>

class Response {
public:
    std::string build(int status, const std::string& body, const std::string& content_type);
};


std::string buildBody(int code, const std::string& message);

#endif