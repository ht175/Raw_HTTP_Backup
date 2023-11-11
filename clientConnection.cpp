
#include "clientConnection.h"
#include <string>


/**
 * @class ClientConnection
 * @brief Represents a connection with a client.
 * 
 * This class encapsulates the details of a client connection,
 * including the IP address and socket file descriptor.
 */

/**
 * @brief Constructor for the ClientConnection class.
 * 
 * Initializes a ClientConnection object with the provided IP address and socket file descriptor.
 *
 * @param ip The IP address of the client connection.
 * @param socket_fd The socket file descriptor associated with this client connection.
 */

ClientConnection::ClientConnection(const std::string ip, int socket_fd):ip(ip),socket_fd(socket_fd){

}


/**
 * @brief Destructor for the ClientConnection class.
 * 
 * Closes the socket associated with the client connection to release the resources.
 */
ClientConnection::~ClientConnection(){
    close(this->socket_fd);
}

/**
 * @brief Retrieves the IP address of the client connection.
 * 
 * @return std::string The IP address of the client as a string.
 */
std::string ClientConnection::getIp() const{
    return this->ip;
}


/**
 * @brief Retrieves the socket file descriptor of the client connection.
 * 
 * @return int The socket file descriptor associated with the client connection.
 */
int ClientConnection::getSocket() const {
    return this->socket_fd;
}