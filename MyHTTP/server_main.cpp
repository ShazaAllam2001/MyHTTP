#include "Server.cpp"

int main(int argc, char* argv[])
{
    if(argc < 2) {
        perror("Port number is not provided");
        exit(EXIT_FAILURE);
    }

    int portNo = atoi(argv[1]);
    Server server = Server(portNo);
    return 0;
}
