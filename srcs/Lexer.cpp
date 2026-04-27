
#include "Lexer.hpp"
#include <cctype>   // std::isspace
 
Lexer::Lexer(const std::string& input)
    : _input(input), _pos(0), _line(1) {}
 
char Lexer::peek() const {
    if (_pos >= _input.size())
        return '\0';
    return _input[_pos];
}
 
char Lexer::advance() {
    char c = _input[_pos++];
    if (c == '\n')
        _line++;
    return c;
}
 
void Lexer::skipWhitespace() {
    while (_pos < _input.size() && std::isspace(peek()))
        advance();
}
 
void Lexer::skipComment() {
    while (_pos < _input.size() && peek() != '\n')
        advance();
    if (_pos < _input.size())
        advance();
}
 
Token Lexer::readWord() {
    std::string word;
    int startLine = _line;
 
    while (_pos < _input.size()) {
        char c = peek();
        if (std::isspace(c) || c == '{' || c == '}' || c == ';' || c == '#')
            break;
        word += advance();
    }
    if (word.empty())
        throw std::runtime_error("Lexer: unexpected empty token");
    return Token(TOKEN_WORD, word, startLine);
}
 
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
 
    while (true) {
        skipWhitespace();
 
        if (_pos >= _input.size()) {
            tokens.push_back(Token(TOKEN_EOF, "", _line));
            break;
        }
 
        char c = peek();
 
        if (c == '#') {
            skipComment();
            continue;
        }
        else if (c == '{') {
            advance();
            tokens.push_back(Token(TOKEN_LBRACE, "{", _line));
        }
        else if (c == '}') {
            advance();
            tokens.push_back(Token(TOKEN_RBRACE, "}", _line));
        }
        else if (c == ';') {
            advance();
            tokens.push_back(Token(TOKEN_SEMICOLON, ";", _line));
        }
        else {
            tokens.push_back(readWord());
        }
    }
 
    return tokens;
}
