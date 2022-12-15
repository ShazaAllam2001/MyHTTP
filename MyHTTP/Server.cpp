#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server_thread.h"

#define SERVER_BACKLOG 20

/* Helper Functions */

bool close_connection() {
    return false;
}

void close_client(int client_socket) {
    close(client_socket);
}

void* open_channel(void* args) {
    Server_thread* server_thread = (Server_thread*) args;

    int valread, valwrite;
    char buffer[1024] = { 0 };
    while(!close_connection()) {
        // read from client
        valread = read(server_thread->client_socket, buffer, sizeof(buffer));
        if(valread < 0) {
            perror("Error: can not read from client");
        }
        printf("%s", buffer);
        bzero(buffer, sizeof(buffer)); // flush buffer

        // write to client
        const char* hello = "Hello from server";
        valwrite = write(server_thread->client_socket, hello, strlen(hello));
        if(valwrite < 0) {
            perror("Error: can not write to client");
        }
        printf("Hello message sent\n");
    }
    // closing the connected (client) socket
    close_client(server_thread->client_socket);
    //free(server_thread); // free args struct

    return nullptr;
}

/* Server class */

class Server {
    public:
        Server(int port) {
            int server_fd;
            struct sockaddr_in address;
            int opt = 1;
            int addrlen = sizeof(address);
            // create the server socket
            /*
                The socket uses processes connected by IPV4 (AF_INET)
                & uses a reliable connection TCP (SOCK_STREAM)
            */
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
                perror("Error: can not set socket");
                exit(EXIT_FAILURE);
            }
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;
            address.sin_port = htons(port);

            // bind socket to port defined
            if(bind(server_fd,(struct sockaddr*)&address,sizeof(address)) < 0) {
                perror("Error: bind failed");
                exit(EXIT_FAILURE);
            }

            // listen to clients
            if(listen(server_fd,SERVER_BACKLOG) < 0) {
                perror("Error: can not listen to clients");
                exit(EXIT_FAILURE);
            }

            while(1) {
                int client_socket;
                if((client_socket = accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addrlen)) < 0) {
                    perror("Error: can not accept to clients");
                    exit(EXIT_FAILURE);
                }

                pthread_t thread;
                Server_thread* args = (Server_thread*)malloc(sizeof(Server_thread));
                args->client_socket = client_socket;
                // open input/output channel thread
                if(pthread_create(&thread, NULL, &open_channel, (void*)args) != 0) {
                     perror("Failed to create thread");
                }
            }

            // closing the listening (server) socket
            shutdown(server_fd, SHUT_RDWR);
        }

        virtual ~Server() {}


    protected:

    private:

};
