#include "handling_files.hpp"
#include "handling_input.hpp"

std::vector<std::string> parse_request(char* request) {
    std::string request_str = request;
    std::string delimeter = " ";
    std::vector<std::string> result;
    std::string subStr;
    int startIndex = 0;
    int  endIndex = 0;
    for(unsigned i=0; i<2; ++i) {
        if((endIndex = request_str.find(delimeter, startIndex)) < request_str.size()) {
            subStr = request_str.substr(startIndex, endIndex - startIndex);
            result.push_back(subStr);
            startIndex = endIndex + delimeter.size();
        }
    }
    if(startIndex < request_str.size()) {
        subStr = request_str.substr(startIndex);
        result.push_back(subStr);
    }
    return result;
}

std::string execute_request(std::vector<std::string> requestTokens) {
    std::string output;
    if(requestTokens.size() > 0) {
        std::string request_type = requestTokens.at(0);
        if(request_type==GET) {
            std::string file_path = requestTokens.at(1);
            output = OK + request_type + "\n" + file_path + "\n" + send_file(file_path);
        }
        else if(request_type==POST) {
            std::string file_path = requestTokens.at(1);
            output = OK + request_type + "\n" + file_path + "\n";
        }
        else if(request_type==SAVE) {
            std::string file_name = requestTokens.at(1);
            std::string file_content = requestTokens.at(2);
            printf("name %s\n",file_name.c_str());
            printf("content %s\n",file_content.c_str());
            save_file(file_content, SERVER_DIR, file_name);
            output = "";
        }
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
    for(unsigned i=0; i<3; ++i) {
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

std::string execute_response(std::vector<std::string> responseTokens) {
    std::string output;
    if(responseTokens.size() > 0) {
        std::string response_msg = responseTokens.at(0);
        if(response_msg == OK_CUT) {
            std::string response_type = responseTokens.at(1);
            std::string file_path = responseTokens.at(2);
            // save file
            if(response_type == GET) {
                save_file(responseTokens.at(3), CLIENT_DIR, getFileName(file_path));
                output = "";
            }
            // send file
            else if(response_type == POST) {
                output = "save " + getFileName(file_path) + " " + send_file(file_path);
            }
        }
    }
    return output;
}
