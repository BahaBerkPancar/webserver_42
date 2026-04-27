
#ifndef PARSER_HPP
#define PARSER_HPP
 
#include "Config.hpp"
#include <vector>
#include <stdexcept>
 
class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
 
    // Parse tokens and return Config structure
    Config parse();
 
private:
    std::vector<Token> _tokens;
    size_t             _pos;
 
    Token peek() const;                    // Peek at current token without advancing
    Token advance();                       // Get current token and advance position
    Token expect(TokenType type);          // Expect token type or throw error
 
    ServerConfig   parseServer();          // Parse server block
    LocationConfig parseLocation();        // Parse location block
    void parseServerDirective(ServerConfig& server);      // Parse directive inside server block
    void parseLocationDirective(LocationConfig& location); // Parse directive inside location block
 
    size_t parseSize(const std::string& val) const;
    std::string intToStr(int n) const;
};
 
#endif
 
