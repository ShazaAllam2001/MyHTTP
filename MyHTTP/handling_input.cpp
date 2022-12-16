#include <vector>
#include <string>
#include <sstream>

vector<string> parse_input(char* input) {
    string input_string = input;
    vector<string> result;
    stringstream s_stream(input); //create string stream from the string
    while(s_stream.good()) {
        string subStr;
        getline(s_stream, subStr, ' '); //get first string delimited by space
        result.push_back(subStr);
    }
    return result;
}

string execute_input(vector<string> input) {
    string output;
    if(input.at(0)=="client_get") {
        output = send_file(input.at(1));
    }
    else if(input.at(0)=="client_post") {
        output = "Hello from server";
    }
    return output;
}
