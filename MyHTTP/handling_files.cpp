#include <string.h>
#include <fstream>

using namespace std;

string send_file(string file_path) {
    ifstream file(&file_path[0], ios::in | ios::binary);
    file.seekg(0, ios::end);
    int file_size = file.tellg();
    file.seekg(0, ios::beg);

    char buffer[file_size] = { 0 };
    string output;
    if(file) { // file exists
        file.read(buffer, file_size); // read file
        string buffer_str = buffer;
        output = "HTTP/1.1 200 OK\r\n" + buffer_str;
    }
    else { // file does not exist
        output = "HTTP/1.1 404 Not Found\r\n";
    }
    return output;
}
