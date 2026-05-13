#include "../include/ServerManager.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

//;
Server_Manager::Server_Manager(Config &Conf) : Conf(Conf), running(false){
    init_listeners();
}

Server_Manager::~Server_Manager(){}

void Server_Manager::printConfig()
{
    (void)Conf;
}

void Server_Manager::init_listeners(){
    for (size_t i = 0; i < Conf.servers.size(); ++i){
        const ServerConfig &S_config = Conf.servers[i];
        for (size_t j = 0; j < S_config.listen.size(); j++){
            ServerSocket socket;
            socket.host = S_config.listen[j].first;
            socket.port = S_config.listen[j].second;
            socket.index = static_cast<int>(i);
            socket.fd = create_Listen_socket(socket.port, socket.host);
            if (listen(socket.fd, SOMAXCONN) == -1){
                close(socket.fd);
                throw std::runtime_error("listen() failed on port " + std::to_string(socket.port));
            }
            listeners.push_back(socket);
            std::cout << "[webserver] Listening on " << socket.host << ":" << socket.port << "\n";
        }
    }
}

static void setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        throw std::runtime_error("fcntl F_GETFL failed");
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
        throw std::runtime_error("fcntl F_SETFL failed");
}

int Server_Manager::create_Listen_socket(int port, std::string host){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
        throw std::runtime_error("Socket() Error");
    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setNonBlocking(fd);

    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(static_cast<uint16_t>(port));
    if (host.empty() || host == "0.0.0.0")
        addr.sin_addr.s_addr = INADDR_ANY;
    else
        addr.sin_addr.s_addr = inet_addr(host.c_str());
    if (bind(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == -1){
        close(fd);
        throw std::runtime_error("bind() failed for port " + std::to_string(port));
    }
    return(fd);
}

void Server_Manager::run(){
    running = true;
}