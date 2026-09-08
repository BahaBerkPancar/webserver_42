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

std::string statusText(int code)
{
    switch (code)
    {
        case 200: return "OK";
        case 201: return "Created";
        case 204: return "No Content";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 400: return "Bad Request";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 413: return "Payload Too Large";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        default:  return "Unknown";
    }
}

std::string Response::build(int status, const std::string& body, const std::string& content_type)
{
    std::ostringstream res;
    res << "HTTP/1.1 " << status << " " << statusText(status) << "\r\n";
    res << "Content-Type: " << content_type << "\r\n";
    res << "Content-Length: " << body.size() << "\r\n";
    res << "Connection: close\r\n";
    res << "\r\n";
    res << body;
    return res.str();
}

std::string Response::buildRedirect(int status, const std::string& location)
{
    std::string body = buildBody(status, statusText(status));
    std::ostringstream res;
    res << "HTTP/1.1 " << status << " " << statusText(status) << "\r\n";
    res << "Location: " << location << "\r\n";
    res << "Content-Type: text/html\r\n";
    res << "Content-Length: " << body.size() << "\r\n";
    res << "Connection: close\r\n";
    res << "\r\n";
    res << body;
    return res.str();
}