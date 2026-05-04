#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP
//;
#include <vector>
#include <map>
#include <poll.h>
#include "ServerSocket.hpp"
#include "Client.hpp"
#include "Config.hpp"

class Server_Manager{
    private:
        const Config &Conf;
    public:
        explicit    Server_Manager(Config &Conf);
        ~Server_Manager();
        void printConfig();
};

#endif