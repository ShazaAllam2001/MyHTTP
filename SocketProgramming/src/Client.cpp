#include <sys/socket.h>
#include <arpa/inet.h>

#include "Client.h"

Client::Client(int server_port)
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr;

    // create the client socket
    /*
        The socket uses processes connected by IPV4 (AF_INET)
        & uses a reliable connection TCP (SOCK_STREAM)
    */
    if(server_fd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("Error: socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    // Convert IPv4 addresses from text to binary form
    if(inet_pton(AF_INET, "localhost",&server_addr.sin_addr) <= 0) {
        perror("Error: Invalid address\n");
        exit(EXIT_FAILURE);
    }

    // connect to the server
    if((client_fd = connect(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) {
        perror("Error: Connection Failed \n");
        exit(EXIT_FAILURE);
    }

    ///////
    int valread;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };
    send(sock, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);

    // closing the connected socket
    close(client_fd);
    ///////
}

Client::~Client()
{
    //dtor
}
