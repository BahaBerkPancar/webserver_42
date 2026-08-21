#include "../include/Response.hpp"
#include <sstream>

std::string buildBody(int code, const std::string& message)
{
    std::ostringstream body;
    body << "<html>\n";
    body << "<head><title>" << code << "</title></head>\n";
    body << "<body>\n";
    body << "<h1>" << code << " " << message << "</h1>\n";
    body << "</body>\n";
    body << "</html>\n";
    return body.str();
}

std::string Response::build(int status, const std::string& body, const std::string& content_type)
{
    std::ostringstream res;
    res << "HTTP/1.1 " << status << " OK\r\n";
    res << "Content-Type: " << content_type << "\r\n";
    res << "Content-Length: " << body.size() << "\r\n";
    res << "Connection: close\r\n";
    res << "\r\n";
    res << body;
    return res.str();
}