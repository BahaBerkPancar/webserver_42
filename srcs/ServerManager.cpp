#include "../include/ServerManager.hpp"
#include <iostream>
//;
Server_Manager::Server_Manager(Config &Conf) : Conf(Conf){}

Server_Manager::~Server_Manager(){}

void Server_Manager::printConfig()
{
    (void)Conf;
}