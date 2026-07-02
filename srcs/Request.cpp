#include "../include/Request.hpp"

#include <iostream>
Request::Request() : done(false){}

bool Request::parse(const std::string& raw)
{
    size_t end = raw.find("\r\n\r\n");
    if (end == std::string::npos)
        return false;
    size_t first_line_end = raw.find("\r\n");
    std::string request_line = raw.substr(0, first_line_end);
    size_t first_space = request_line.find(' ');
    size_t second_space = request_line.find(' ', first_space + 1);
    if (first_space == std::string::npos || second_space == std::string::npos)
        return false;
    method  = request_line.substr(0, first_space);
    path    = request_line.substr(first_space + 1, second_space - first_space - 1);
    version = request_line.substr(second_space + 1);
    size_t pos = first_line_end + 2;
    while (pos < end)
    {
        size_t line_end = raw.find("\r\n", pos);
        std::string line = raw.substr(pos, line_end - pos);
        size_t colon = line.find(": ");
        if (colon != std::string::npos)
            headers[line.substr(0, colon)] = line.substr(colon + 2);
        pos = line_end + 2;
    }
    req_body = raw.substr(end + 4);
    done = true;
    return true;
}