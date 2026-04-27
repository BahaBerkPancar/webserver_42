
#include "Lexer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
 
// Token tipini string'e çeviren yardımcı
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
        std::cerr << "Kullanim: ./webserv <config_dosyasi>\n";
        return 1;
    }
 
    // Dosyayı oku
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Hata: dosya acilamadi: " << argv[1] << "\n";
        return 1;
    }
 
    std::ostringstream ss;
    ss << file.rdbuf();
    std::string content = ss.str();
 
    // Lexer'ı çalıştır
    try {
        Lexer lexer(content);
        std::vector<Token> tokens = lexer.tokenize();
 
        // Token'ları ekrana bas (test amaçlı)
        for (size_t i = 0; i < tokens.size(); i++) {
            std::cout << "satir:" << tokens[i].line
                      << "  tip:" << tokenTypeName(tokens[i].type)
                      << "  deger:'" << tokens[i].value << "'\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Hata: " << e.what() << "\n";
        return 1;
    }
 
    return 0;
}
 
