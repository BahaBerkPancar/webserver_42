
#include "Lexer.hpp"
#include "Parser.hpp"
#include "../include/ServerManager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./webserv <config_file>\n";
        return 1;
    }

    try {
        // Read configuration file
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cerr << "Error: failed to open file: " << argv[1] << "\n";
            return 1;
        }

        std::ostringstream ss;
        ss << file.rdbuf();
        file.close();
        std::string content = ss.str();

        // Tokenize
        Lexer lexer(content);
        std::vector<Token> tokens = lexer.tokenize();

        // Parse
        Parser parser(tokens);
        Config config = parser.parse();

        std::cout << "Config loaded: " << config.servers.size() << " server(s)\n";
        Server_Manager a(config);
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
