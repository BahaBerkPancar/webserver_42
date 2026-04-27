
#include "Lexer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
 
// Convert token type to string for debugging
static std::string tokenTypeName(TokenType t) {
    switch (t) {
        case TOKEN_WORD:      return "WORD";
        case TOKEN_LBRACE:    return "LBRACE";
        case TOKEN_RBRACE:    return "RBRACE";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_EOF:       return "EOF";
        default:              return "UNKNOWN";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./webserv <config_file>\n";
        return 1;
    }
 
    // Read configuration file
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: failed to open file: " << argv[1] << "\n";
        return 1;
    }
 
    std::ostringstream ss;
    ss << file.rdbuf();
    std::string content = ss.str();
 
    try {
        Lexer lexer(content);
        std::vector<Token> tokens = lexer.tokenize();
 
        for (size_t i = 0; i < tokens.size(); i++) {
            std::cout << "line:" << tokens[i].line
                      << "  type:" << tokenTypeName(tokens[i].type)
                      << "  value:'" << tokens[i].value << "'\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
 
    return 0;
}
 
