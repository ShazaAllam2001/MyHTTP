#include <pthread.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "server_thread.h"
#include "handling_input.hpp"

#define SERVER_BACKLOG 20
#define BUFFER_SIZE 1048576 // 1 MB
#define REQUESTS_PER_SECOND 100 // server can handle to 100 requests per second

/* Helper Functions */

bool close_connection(int num_requests, double time_consumed, int num_clients) {
    // calculate the number of requests need per second
    // assuming that all other clents are using the server like that client
    // so for fairness other should be able to use the server at least like that do,
    // then if for achieving that we exceed the REQUESTS_PER_SECOND threshold we close the connection
    double reqPerSec = (num_requests/time_consumed)*num_clients;
    if(reqPerSec > REQUESTS_PER_SECOND) {
        return true;
    }
    return false;
}

void close_client(int client_socket) {
    close(client_socket);
}

void* open_channel(void* args) {
    Server_thread* server_thread = (Server_thread*) args;
    struct timeval stop, start;
    double time_consumed;
    int num_requests = 0;
    gettimeofday(&start, NULL);

    int valread, valwrite;
    char buffer[BUFFER_SIZE] = { 0 };
    while(!close_connection(num_requests,time_consumed,*server_thread->server_clients)) {
        // read from client
        valread = read(server_thread->client_socket, buffer, sizeof(buffer));
        if(valread < 0) {
            perror("Error: can not read from client");
        }
        else if(valread > 0) {
            gettimeofday(&stop, NULL);
            time_consumed = stop.tv_sec - start.tv_sec;
            if(buffer != "") {
                num_requests++;
                printf("%s\n", buffer);
            }
        }

        // evaluate input
        std::vector<std::string> request = parse_request(buffer);
        bzero(buffer, sizeof(buffer)); // flush buffer
        std::string output_str = execute_request(request);
        const char* output = output_str.c_str();
        if(output != "") {
            printf("%s\n", output);
        }

        // write to client
        valwrite = write(server_thread->client_socket, output, strlen(output));
        if(valwrite < 0) {
            perror("Error: can not write to client");
        }
    }

    // closing the connected (client) socket
    close_client(server_thread->client_socket);
    //free(server_thread); // free args struct
    return nullptr;
}

/* Server class */

class Server {
    public:
        int num_clients;

        Server(int port) {
            this->num_clients = 0; // intial number of clients

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

                this->num_clients += 1;
                // thread for a new client is created
                pthread_t thread;
                Server_thread* args = (Server_thread*)malloc(sizeof(Server_thread));
                args->client_socket = client_socket;
                args->server_clients = &this->num_clients;
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
