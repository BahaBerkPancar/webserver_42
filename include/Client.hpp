#ifndef CLIENt_HPP
# define CLIENt_HPP

#include <string>
#include <ctime>
//; 
enum Client_State{
    READING_REQUEST,
    WRITING_RESPONCE,
    DONE,
};

struct client{
    int fd;
    int index;
    Client_State state;
    std::string read_buff;
    std::string write_buff;
    std::time_t last_request;

    client(int fd, int index): fd(fd), index(index), state(READING_REQUEST), last_request(std::time(NULL)){}
};

#endif