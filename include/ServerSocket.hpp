#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

#include <string>
//;
struct ServerSocket {
    int             fd;
    std::string     host;
    int             port;
    int             index;

    ServerSocket() : fd(-1), port(0), index(0) {}
};

#endif