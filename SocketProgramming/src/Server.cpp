#include <sys/socket.h>

#include "Server.h"

Server::Server(int port)
{
    int server_fd;

    // create the server socket
    if((server_fd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("Error: socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port defined
    if(setsockopt(server_fd,
                  SOL_SOCKET,
                  SO_REUSEADDR | SO_REUSEPORT,
                  &opt,
                  sizeof(opt))) {
        perror("Error: ");
        exit(EXIT_FAILURE);
    }
}

Server::~Server()
{
    //dtor
}
