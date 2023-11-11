#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sstream>
#include "exception.h"

class Client{
    private:
    std::string target_ip;
    std::string target_port;
    int socket_fd;
    public:
    Client(std::string ip, std::string port);
    ~Client();
    void initialize();
    int socketCreation(struct addrinfo* host_info_list);
    void connectToTarget(struct addrinfo* host_info_list, int fd);
};




#endif // CLIENT_H