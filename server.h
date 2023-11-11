#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "clientConnection.h" 
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

class Server {
public:
    // Constructor
    Server(const std::string hostname, const std::string port);

    // Destructor
    ~Server();

    // Initialize the server (e.g., create and bind the socket)
    void initialize();

    // Create a socket
    int socketCreation(struct addrinfo* host_info_list);

    // Bind the socket
    void bindSocket(struct addrinfo* host_info_list);

    // Listen for client connections
    void listenClient(int socket_fd);

    // Accept a new client connection
    ClientConnection* accept_client_request();

    // Getters
    std::string getHostname() const;
    std::string getPort() const;
    int getSocket() const;

private:
    std::string hostname;
    std::string port;
    int socket_fd; // listen Socket file descriptor for the server 
};

#endif // SERVER_H
