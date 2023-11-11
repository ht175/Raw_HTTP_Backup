#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include <string>
#include <unistd.h> // For close function
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>

class ClientConnection {
private:
    std::string ip;      // IP address of the client
    int socket_fd;       // Socket file descriptor for the connection

public:
    // Constructor
    
    ClientConnection(const std::string ip, int socket_fd);

    // Destructor
    ~ClientConnection();

    // Getter for IP address
    std::string getIp() const;

    // Getter for socket file descriptor
    int getSocket() const;
};

#endif // CLIENT_CONNECTION_H
