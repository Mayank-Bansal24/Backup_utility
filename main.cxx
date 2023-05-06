#include "main.hxx"



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
    for (const auto& arg : command_args) {
        std::cout << arg.first << " -> " << arg.second << std::endl;
    }
}

int main(int argc, char* argv[]) {
    execute_command(argc, argv);
    return 0;
}