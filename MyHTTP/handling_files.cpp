#include <string.h>
#include <fstream>
#include <filesystem>

#include "handling_files.hpp"

std::string getFileName(std::string file_path) {
    std::string file_name = std::filesystem::path(file_path).filename();
    return file_name;
}

std::string send_file(std::string file_path) {
    std::ifstream file(&file_path[0], std::ios::in | std::ios::binary);
    file.seekg(0, std::ios::end);
    int file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    char buffer[file_size] = { 0 };
    std::string output;
    if(file) { // file exists
        file.read(buffer, file_size); // read file
        output = buffer;
    }
    else { // file does not exist
        output = NOTFOUND;
    }
    file.close();
    return output;
}

void save_file(std::string file_content, std::string file_dir, std::string file_name) {
    const char* file_content_ch = file_content.c_str();
    std::ofstream file(file_dir + file_name, std::ios::out | std::ios::binary);
    if(file) {
        file.write(file_content_ch, strlen(file_content_ch)); // write file
    }
    file.close();
}
