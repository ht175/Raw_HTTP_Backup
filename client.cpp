#include "client.h"

Client::Client(std::string ip, std::string port):target_ip(ip),target_port(port),socket_fd(-1){
    initialize();
}
Client::~Client(){
    if(socket_fd!=-1){
         close(socket_fd); 
    }
}

void Client::initialize(){
    struct addrinfo *host_info_list;
    try{
        this->socket_fd = socketCreation(host_info_list);
        connectToTarget(host_info_list, this->socket_fd);
        freeaddrinfo(host_info_list);
       
    }catch(const SocketConnException &e){
       freeaddrinfo(host_info_list);
       if(this->socket_fd!=-1){
        close(socket_fd);
       }
       throw;
    }
    freeaddrinfo(host_info_list);
}

int Client::socketCreation(struct addrinfo* host_info_list){
    int status;
    int socket_fd;
    struct addrinfo host_info;
    const char *port = this->target_port.c_str();

    memset(&host_info, 0, sizeof(host_info));

    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(this->target_ip.c_str(), port, &host_info, &host_info_list);
    if (status != 0) {
        std::ostringstream errorMessage;
        errorMessage << "Client cannot get address info for target"
                     << "(" << this->target_ip << "," << this->target_port << ")";
        throw SocketConnException(errorMessage.str());
        
    } //if

    socket_fd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
    if (socket_fd == -1) {
        std::ostringstream errorMessage;
        errorMessage<< "Client cannot create socket for target" 
                    << "  (" << this->target_ip << "," << this->target_port << ")";
       throw SocketConnException(errorMessage.str());
    } 
    return socket_fd;
}


void Client::connectToTarget(struct addrinfo* host_info_list, int fd){
    int status = connect(fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1) {
        std::ostringstream errorMessage;
        errorMessage << "Error: cannot connect to socket" 
                    << "  (" << this->target_ip << "," << this->target_port << ")";
        throw SocketConnException(errorMessage.str());
    } 
}