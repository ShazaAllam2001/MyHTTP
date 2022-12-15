#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Client {
    public:
        Client(const char* server_ip, int server_port) {
            int server_fd, client_fd;
            struct sockaddr_in server_addr;

            // create the client socket
            /*
                The socket uses processes connected by IPV4 (AF_INET)
                & uses a reliable connection TCP (SOCK_STREAM)
            */
            if((server_fd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
                perror("Error: socket creation failed\n");
                exit(EXIT_FAILURE);
            }

            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(server_port);

            // Convert IPv4 addresses from text to binary form
            if(inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr) <= 0) {
                perror("Error: Invalid address\n");
                exit(EXIT_FAILURE);
            }

            // connect to the server
            if((client_fd = connect(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) {
                perror("Error: Connection Failed \n");
                exit(EXIT_FAILURE);
            }

            // open input/output chennel
            open_channel(server_fd);

            // closing the connected socket
            close(client_fd);
        }

        virtual ~Client() {}

        void open_channel(int server_fd) {
            int valread, valwrite;
            char buffer[1024] = { 0 };
            while(1) {
                fgets(buffer, sizeof(buffer), stdin); // read input from user
                valwrite = write(server_fd, buffer, strlen(buffer)); // send input to server
                if(valwrite < 0) {
                    perror("Error: can not write to server");
                    exit(EXIT_FAILURE);
                }
                bzero(buffer, sizeof(buffer)); // flush buffer

                valread = read(server_fd, buffer, 1024); // read response from server
                if(valread < 0) {
                    perror("Error: can not read from server");
                    exit(EXIT_FAILURE);
                }
                printf("%s\n", buffer);
                bzero(buffer, sizeof(buffer)); // flush buffer
            }
        }

    protected:

    private:
};