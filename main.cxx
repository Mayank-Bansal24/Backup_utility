#include "main.hxx"

namespace fs = boost::filesystem;

void find_path(const std::string& file, std::vector<std::string>& path) {
    // implementation of git add function
    std::cout << "Finding path of file: " << file << std::endl;
    fs::path current_path = fs::current_path();
    fs::path file_path = current_path / file;
    if (fs::exists(file_path)) {
        path.push_back(file_path.string());
    } else {
        std::cout << "File not found: " << file << std::endl;
    }
}

void execute_command(int argc, char* argv[]) {
    std::map<std::string, std::string> command_args;
    int num_args_to_parse = std::min(argc - 1, 2); // only parse first two arguments
    for (int i = 1; i <= num_args_to_parse; ++i) {
        if (i < argc - 1) {
            command_args[argv[i]] = argv[i + 1];
            ++i;
        } else {
            command_args[argv[i]] = "";
        }
    }
    std::vector<std::string> path;
    for (const auto& arg : command_args) {
        std::cout << "Map formed: "<<arg.first << " -> " << arg.second << std::endl;
        if (arg.first == "path") {
            find_path(arg.second, path);
        }
        else {
            std::cout << "Unidentified function" << std::endl;
        }
    }
    for (const auto& p : path) {
        std::cout << "Path: " << p << std::endl;
    }
}

int main(int argc, char* argv[]) {
    execute_command(argc, argv);
    return 0;
}