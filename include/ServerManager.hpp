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
        bool running;
        std::vector<ServerSocket>   listeners;
        std::map<int, Client>       _clients;
        static const int    TIMEOUT_MS      = 5000;
        static const int    CLIENT_TIMEOUT  = 60;
        
        void init_listeners();
        int create_Listen_socket(int, std::string);

    public:
        explicit    Server_Manager(Config &Conf);
        ~Server_Manager();
        void printConfig();
        void run();
};

#endif