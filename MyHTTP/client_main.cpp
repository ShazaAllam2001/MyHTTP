#include "Client.cpp"

int main(int argc, char* argv[])
{
    if(argc < 3) {
        perror("IP or Port number are not provided");
        exit(EXIT_FAILURE);
    }

    int portNo = atoi(argv[2]);
    Client client = Client(argv[1], portNo);
    return 0;
}
