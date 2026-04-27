
#ifndef LEXER_HPP
#define LEXER_HPP
 
#include "Config.hpp"
#include <string>
#include <vector>
#include <stdexcept>
 
class Lexer {
public:
    explicit Lexer(const std::string& input);
 
    // Tüm token'ları üretip döndürür
    std::vector<Token> tokenize();
 
private:
    std::string _input;
    size_t      _pos;
    int         _line;
 
    char  peek() const;      // mevcut karaktere bak, ilerleme
    char  advance();         // karakteri al ve ilerle
    void  skipWhitespace();  // boşluk / tab / newline atla
    void  skipComment();     // '#' dan satır sonuna kadar atla
    Token readWord();        // boşluk / { / } / ; gelene kadar oku
};
 
#endif
