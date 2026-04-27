
#ifndef LEXER_HPP
#define LEXER_HPP
 
#include "Config.hpp"
#include <string>
#include <vector>
#include <stdexcept>
 
class Lexer {
public:
    explicit Lexer(const std::string& input);
 
    // Tokenize input and return all tokens
    std::vector<Token> tokenize();
 
private:
    std::string _input;
    size_t      _pos;
    int         _line;
 
    char  peek() const;      // Peek at current character without advancing
    char  advance();         // Get current character and advance position
    void  skipWhitespace();  // Skip whitespace characters
    void  skipComment();     // Skip comment line (from # to newline)
    Token readWord();        // Read word until whitespace or special char
};
 
#endif
