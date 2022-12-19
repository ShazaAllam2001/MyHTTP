#include <string>
#include <vector>

static std::string OK = "HTTP/1.1 200 OK\r\n";
static std::string NOTFOUND = "HTTP/1.1 404 Not Found\r\n";

static std::string OK_CUT = "HTTP/1.1 200 OK\r";
static std::string NOTFOUND_CUT = "HTTP/1.1 404 Not Found\r";

static std::string SERVER_DIR = "./my_server_folder/";
static std::string CLIENT_DIR = "./my_client_folder/";

std::string getFileName(std::string file_path);

std::string send_file(std::string file_path);

void save_file(std::string file_content, std::string file_dir, std::string file_name);
