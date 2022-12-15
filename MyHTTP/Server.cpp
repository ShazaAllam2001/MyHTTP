#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Server {
    public:
        Server(int port) {
            int server_fd, client_socket;
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
            if(listen(server_fd,20) < 0) {
                perror("Error: can not listen to clients");
                exit(EXIT_FAILURE);
            }

            /*if(pthread_create(&thread[i], NULL, &routine_2, (void*)args) != 0) {
                perror("Failed to create thread");
            }*/

            if((client_socket = accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addrlen)) < 0) {
                perror("Error: can not accept to clients");
                exit(EXIT_FAILURE);
            }

            // open input/output chennel
            open_channel(client_socket);

            // closing the connected (client) socket
            close_client(client_socket);

            // closing the listening (server) socket
            shutdown(server_fd, SHUT_RDWR);
        }

        virtual ~Server() {}

        /*void* accept_client(void* args) {
            int client_socket;
            if((client_socket = accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addrlen)) < 0) {
                perror("Error: can not accept to clients");
                exit(EXIT_FAILURE);
            }

            // open input/output chennel
            open_channel();

            // closing the connected (client) socket
            close_client(client_socket);
        }*/

        void open_channel(int client_socket) {
            int valread;
            char buffer[1024] = { 0 };
            while(!close_connection()) {
                // read from client
                valread = read(client_socket, buffer, sizeof(buffer));
                if(valread < 0) {
                    perror("Error: can not read from client");
                    exit(EXIT_FAILURE);
                }
                printf("%s", buffer);
                bzero(buffer, sizeof(buffer)); // flush buffer

                // write to client
                const char* hello = "Hello from server";
                send(client_socket, hello, strlen(hello), 0);
                printf("Hello message sent\n");
            }
        }

        bool close_connection() {
            return false;
        }

        void close_client(int client_socket) {
            close(client_socket);
        }

    protected:

    private:
};
