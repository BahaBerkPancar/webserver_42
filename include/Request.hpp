#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <map>

class Request {
public:
    std::string method;
    std::string path;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string req_body;
    bool        done;

    Request();
    bool parse(const std::string& raw);
};

#endif