#include <sys/socket.h>
#include <iostream>

using namespace std;

int main()
{
    int server_fd = socket(AF_INET,SOCK_STREAM,0)
    cout << server_fd << endl;
    return 0;
}
