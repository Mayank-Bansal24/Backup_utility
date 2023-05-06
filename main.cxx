#include "main.hxx"



std::vector<std::string> parse_args(int argc, char* argv[]) {
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    return args;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args = parse_args(argc, argv);
    
    for (const auto& arg : args) {
        std::cout << arg << " ";
    }

    return 0;
}