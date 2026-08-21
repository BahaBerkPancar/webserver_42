#ifndef ROUTER_HPP
# define ROUTER_HPP

#include "Config.hpp"
#include "Request.hpp"

class Router {
public:
    const LocationConfig* match(const ServerConfig& server, const std::string& path);
};

#endif