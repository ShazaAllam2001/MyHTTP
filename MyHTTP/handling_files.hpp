#include <string>
#include <vector>

static const std::string OK = "HTTP/1.1 200 OK\r\n";
static const std::string NOTFOUND = "HTTP/1.1 404 Not Found\r\n";

static const std::string OK_CUT = "HTTP/1.1 200 OK\r";
static const std::string NOTFOUND_CUT = "HTTP/1.1 404 Not Found\r";

static const std::string SERVER_DIR = "./my_server_folder/";
static const std::string CLIENT_DIR = "./my_client_folder/";

std::string getFileName(std::string file_path);

std::string send_file(std::string file_path);

void save_file(std::string file_content, std::string file_dir, std::string file_name);
