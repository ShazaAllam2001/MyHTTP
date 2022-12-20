#include <string.h>
#include <fstream>
#include <filesystem>

#include "handling_files.hpp"

std::string getFileName(std::string file_path) {
    std::string file_name = std::filesystem::path(file_path).filename();
    return file_name;
}

std::vector<std::string> send_file(std::string file_path) {
    std::ifstream file(&file_path[0], std::ios::in | std::ios::binary);
    file.seekg(0, std::ios::end);
    int file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    char buffer[file_size] = { 0 };
    std::vector<std::string> output;
    if(file) { // file exists
        file.read(buffer, file_size); // read file
        output.push_back(std::to_string(file_size));
        std::string buffer_str = buffer;
        output.push_back(buffer_str);
    }
    file.close();
    return output;
}

void save_file(std::string file_content, int file_size, std::string file_dir, std::string file_name) {
    const char* file_content_ch = file_content.c_str();
    std::ofstream file(file_dir + file_name, std::ios::out | std::ios::binary);
    if(file) {
        file.write(file_content_ch, file_size); // write file
    }
    file.close();
}
