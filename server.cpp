#include "server.h"


/**
 * @brief Constructor for Server class.
 * 
 * Initializes a Server object with the provided hostname and port.
 *
 * @param hostname The hostname or IP address on which the server will run.
 * @param port The port number on which the server will listen for connections.
 */
Server::Server(const std::string hostname, const std::string port):hostname(hostname),port(port),socket_fd(-1){
    initialize();
}

/**
 * @brief Destructor for Server class.
 * 
 * Closes the server's listening socket to release the resources.
 */
Server::~Server(){ 
    if(socket_fd != -1){
        close(socket_fd);
    }
}

/**
 * @brief Initializes the server to be ready to accept connections.
 * 
 * This method handles the creation and binding of the socket to the local address.
 * It also sets the socket options and starts listening for incoming connections.
 *
 * @throws SocketConnException If an error occurs during the setup process.
 */
void Server::initialize()
{
    struct addrinfo *host_info_list;
    try
    {
        
        int socket_fd = this->socketCreation(host_info_list);
        this->bindSocket(host_info_list);
        freeaddrinfo(host_info_list);
        this->socket_fd = socket_fd;
    }
    catch (const SocketConnException &e)
    {
        freeaddrinfo(host_info_list);
        if(this->socket_fd!=-1){
            close(socket_fd);
        }
        std::cerr << "Error encountered: " << e.what() << std::endl;
        throw;
    }

    // free objects allocated on the heap: freeaddrinfo() after getaddrinfo()
}

/**
 * @brief Creates a socket for the server and returns the file descriptor.
 * 
 * This function sets up the address structures and calls getaddrinfo.
 *
 * @param host_info_list A pointer to addrinfo struct that will hold host information.
 * @return int The file descriptor for the newly created socket.
 * @throws SocketConnException If the socket creation or address retrieval fails.
 */

int Server::socketCreation(struct addrinfo *host_info_list)
{
    int status;
    int socket_fd;
    struct addrinfo host_info;
    const char *hostname = this->hostname.empty() ? nullptr : this->hostname.c_str();
    const char *port = this->hostname.empty() ? nullptr : this->port.c_str();
    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    host_info.ai_flags = AI_PASSIVE;
    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    if (status != 0)
    {
        std::ostringstream errorMessage;
        errorMessage << "Error: cannot get address info for host "
                     << "(" << hostname << "," << port << ")";
        throw SocketConnException(errorMessage.str());
    }
    // if port are not given, assign a random one
    if (port == nullptr)
    {
        struct sockaddr_in *addr = (struct sockaddr_in *)(host_info_list->ai_addr);
        addr->sin_port = 0;
    }
    socket_fd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
    if (socket_fd == -1)
    {
        std::ostringstream errorMessage;
        errorMessage << "cannot create socket for host "
                     << "(" << hostname << "," << port << ")";
        throw SocketConnException(errorMessage.str());
    }
    return socket_fd;
}


/**
 * @brief Binds the server socket with the given address information.
 * 
 * Sets the socket options to reuse the address and binds the server socket.
 *
 * @param host_info_list A pointer to addrinfo struct that contains the address to bind to.
 * @throws SocketConnException If binding the socket fails.
 */
void Server::bindSocket(struct addrinfo *host_info_list)
{
    int yes = 1;
    int status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)
    {
        std::ostringstream errorMessage;
        errorMessage << "cannot bind socket for host "
                     << "(" << hostname << "," << port << ")";
        throw SocketConnException(errorMessage.str());
    }
}

/**
 * @brief Starts listening for client connections on the server socket.
 * 
 * This function will configure the server to accept connections with a specified backlog.
 *
 * @param socket_fd The file descriptor of the server socket to listen on.
 * @throws SocketConnException If the server fails to start listening on the socket.
 */
void Server::listenClient(int socket_fd)
{
    int status = listen(socket_fd, 100);
    if (status == -1)
    {
        std::ostringstream errorMessage;
        errorMessage << "annot listen on socket "
                     << "(" << hostname << "," << port << ")";
        throw SocketConnException(errorMessage.str());
    }
}

/**
 * @brief Accepts a new client connection.
 * 
 * Waits for a client to connect and returns a new ClientConnection object
 * that represents this connection.
 *
 * @return ClientConnection* A pointer to a newly created ClientConnection object.
 * @throws SocketConnException If accepting the client connection fails.
 */
ClientConnection * Server::accept_client_request(){
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int client_connection_fd;
    client_connection_fd = accept(this->socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
    if (client_connection_fd == -1) {
        throw SocketConnException("cannot accept connection on socket");
    } 
    struct sockaddr_in * sockaddr = (struct sockaddr_in *)&socket_addr;

    std::string ip = inet_ntoa(sockaddr->sin_addr);
    return new ClientConnection(ip,client_connection_fd);;
}

/**
 * @brief Retrieves the server's hostname.
 * 
 * @return std::string The hostname of the server.
 */
std::string Server::getHostname() const{
    return this->hostname;
}

/**
 * @brief Retrieves the server's port number.
 * 
 * @return std::string The port number of the server as a string.
 */
std::string Server::getPort() const {
    return this->port;
}

/**
 * @brief Retrieves the server's socket file descriptor.
 * 
 * @return int The file descriptor of the server's listening socket.
 */
int Server::getSocket() const{
    return this->socket_fd;
}
