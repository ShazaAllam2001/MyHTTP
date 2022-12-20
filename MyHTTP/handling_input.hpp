#include <vector>
#include <string>

static const std::string GET = "client_get";
static const std::string POST = "client_post";
static const std::string SAVE = "save";

std::vector<std::string> parse_request(char* input);

std::string execute_request(std::vector<std::string> input);

std::vector<std::string> parse_response(char* response);

std::string execute_response(std::vector<std::string> responseTokens);
