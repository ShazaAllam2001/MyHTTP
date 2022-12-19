#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <fstream>

#include "handling_files.hpp"
#include "handling_input.hpp"

#define BUFFER_SIZE 1048576 // 1 MB

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
            if(inet_pton(AF_INET,server_ip,&server_addr.sin_addr) <= 0) {
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
            char buffer[BUFFER_SIZE] = { 0 };

            // read input file name from user
            char* file_name = (char*)malloc(sizeof(buffer));
            printf("Enter the input file name:\n");
            scanf("%s",file_name);
            std::ifstream input_file(file_name);
            free(file_name);

            // read input file
            std::string line;
            if(input_file.is_open()) {
                while(input_file.good()) {
                    getline(input_file,line);
                    for(int i=0; i<line.size(); ++i) {
                        buffer[i] = line[i];
                    }
                    printf("%s\n", buffer);

                    valwrite = write(server_fd, buffer, sizeof(buffer)); // send input to server
                    if(valwrite < 0) {
                        perror("Error: can not write to server");
                    }
                    bzero(buffer, sizeof(buffer)); // flush buffer

                    valread = read(server_fd, buffer, sizeof(buffer)); // read response from server
                    if(valread < 0) {
                        perror("Error: can not read from server");
                    }
                    printf("%s\n", buffer);

                    // save file
                    std::vector<std::string> response = parse_response(buffer);
                    if(response.at(0) == OK_CUT) {
                        save_file(response.at(2), CLIENT_DIR, response.at(1));
                    }
                    bzero(buffer, sizeof(buffer)); // flush buffer
                }
            }
            /*while(1) {
                fgets(buffer, sizeof(buffer), stdin); // read input from user
                valwrite = write(server_fd, buffer, sizeof(buffer)); // send input to server
                if(valwrite < 0) {
                    perror("Error: can not write to server");
                }
                bzero(buffer, sizeof(buffer)); // flush buffer

                valread = read(server_fd, buffer, sizeof(buffer)); // read response from server
                if(valread < 0) {
                    perror("Error: can not read from server");
                }
                printf("%s\n", buffer);
                bzero(buffer, sizeof(buffer)); // flush buffer
            }*/
        }

    protected:

    private:
};
