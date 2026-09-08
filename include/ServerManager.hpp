#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP
//;
#include <vector>
#include <map>
#include <poll.h>
#include "ServerSocket.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "Router.hpp"
#include "Request.hpp"

class Server_Manager{
    private:
        const Config &Conf;
        bool running;
        Router                      router;
        std::vector<ServerSocket>   listeners;
        std::map<int, Client>       _clients;
        static const int    TIMEOUT_MS      = 5000;
        static const int    CLIENT_TIMEOUT  = 60;

        void init_listeners();
        int create_Listen_socket(int, std::string);
        std::vector <pollfd>create_listener_fds();
        std::string handleRequest(int serverIndex, Request &req);
        std::string serveStatic(const LocationConfig &loc, const std::string &fsPath);
        static std::string getMimeType(const std::string &path);
    public:
        explicit    Server_Manager(Config &Conf);
        ~Server_Manager();
        void printConfig();
        void run();
};

#endif