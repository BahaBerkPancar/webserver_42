#include "../include/ServerManager.hpp"
#include "../include/Request.hpp"
#include "../include/Router.hpp"
#include "../include/Response.hpp"
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

std::vector <pollfd>Server_Manager::create_listener_fds(){
    std::vector <pollfd> ret;
    for (size_t i = 0; i < listeners.size(); ++i)
    {
        pollfd fds;
        fds.fd = listeners[i].fd;
        fds.events = POLLIN;
        fds.revents = 0;
        ret.push_back(fds);
    }
    return(ret);
}

void Server_Manager::run(){
    running = true;
    std::vector <pollfd> listeners_fds = create_listener_fds();

    while(running)
    {
        std::vector <pollfd> fds = listeners_fds;
        std::vector <int> client_fd_order;
        for (std::map <int, Client>::iterator i = _clients.begin(); i != _clients.end(); ++i){
            pollfd poll_fd;
            poll_fd.fd = i->first;
            poll_fd.events = (i->second.state == WRITING_RESPONCE) ? POLLOUT : POLLIN;
            poll_fd.revents = 0;
            fds.push_back(poll_fd);
            client_fd_order.push_back(i->first);
        }
        int poll_ret = poll(fds.data(), fds.size(), TIMEOUT_MS);
        if (poll_ret == -1)
            throw std::runtime_error("poll failed");

        for (size_t i = 0; i < fds.size(); ++i){
            if(i < listeners.size()){
                if(fds[i].revents & POLLIN){
                    struct sockaddr_in client_addr;
                    socklen_t socklen = sizeof(client_addr);
                    int clientfd =  accept(fds[i].fd, reinterpret_cast<sockaddr *> (&client_addr), &socklen);
                    if(clientfd == -1)
                        continue;
                    setNonBlocking(clientfd);
                    _clients.insert(std::make_pair(clientfd, Client(clientfd, listeners[i].index)));
                    std::cout << "[webserver] New connection on fd " << clientfd << " (total clients: " << _clients.size() << ")\n";
                }
                continue;
            }

            std::map<int, Client>::iterator it = _clients.find(fds[i].fd);
            if (it == _clients.end())
                continue;
            Client &client = it->second;

            if (fds[i].revents & (POLLHUP | POLLERR)){
                close(client.fd);
                _clients.erase(it);
                continue;
            }

            if (fds[i].revents & POLLIN){
                char buffer[4096];
                int bytes = recv(client.fd, buffer, sizeof(buffer), 0);
                if (bytes <= 0){
                    close(client.fd);
                    _clients.erase(it);
                    continue;
                }
                client.read_buff.append(buffer, bytes);
                client.last_request = std::time(NULL);
                if (client.read_buff.find("\r\n\r\n") != std::string::npos)
                {
                    Request req;
                    req.parse(client.read_buff);
                    Response res;
                    client.write_buff = res.build(200, "<h1>Hello from webserv!</h1>", "text/html");
                    client.read_buff.clear();
                    client.state = WRITING_RESPONCE;
                }
            }
            else if (fds[i].revents & POLLOUT){
                int bytes = send(client.fd, client.write_buff.c_str(), client.write_buff.size(), 0);
                if (bytes <= 0){
                    close(client.fd);
                    _clients.erase(it);
                    continue;
                }
                client.write_buff.erase(0, bytes);
                if (client.write_buff.empty()){
                    close(client.fd);
                    _clients.erase(it);
                }
            }
        }
    }
}