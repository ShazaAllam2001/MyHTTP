#include <sstream>

#include "handling_files.hpp"
#include "handling_input.hpp"

std::vector<std::string> parse_request(char* request) {
    std::vector<std::string> result;
    std::stringstream s_stream(request); //create string stream from the string
    while(s_stream.good()) {
        std::string subStr;
        getline(s_stream, subStr, ' '); //get first string delimited by space
        result.push_back(subStr);
    }
    return result;
}

std::string execute_request(std::vector<std::string> requestTokens) {
    std::string output;
    if(requestTokens.at(0)=="client_get") {
        output = send_file(requestTokens.at(1));
    }
    else if(requestTokens.at(0)=="client_post") {
        //output = "Hello from server";
        output = send_file(requestTokens.at(1));
    }
    return output;
}

std::vector<std::string> parse_response(char* response) {
    std::string response_str = response;
    std::string delimeter = "\n";
    std::vector<std::string> result;
    std::string subStr;
    int startIndex = 0;
    int  endIndex = 0;
    for(unsigned i=0; i<2; ++i) {
        if((endIndex = response_str.find(delimeter, startIndex)) < response_str.size()) {
            subStr = response_str.substr(startIndex, endIndex - startIndex);
            result.push_back(subStr);
            startIndex = endIndex + delimeter.size();
        }
    }
    if(startIndex < response_str.size()) {
        subStr = response_str.substr(startIndex);
        result.push_back(subStr);
    }
    return result;
}
