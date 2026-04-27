
#ifndef PARSER_HPP
#define PARSER_HPP
 
#include "Config.hpp"
#include <vector>
#include <stdexcept>
 
class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
 
    // Token listesini okuyup Config struct'ını doldurur
    Config parse();
 
private:
    std::vector<Token> _tokens;
    size_t             _pos;
 
    Token peek() const;                    // mevcut token'a bak
    Token advance();                       // token'ı al ve ilerle
    Token expect(TokenType type);          // beklenen token gelmezse hata fırlat
 
    ServerConfig   parseServer();          // server { } bloğunu parse et
    LocationConfig parseLocation();        // location / { } bloğunu parse et
    void parseServerDirective(ServerConfig& server);      // server içi direktif
    void parseLocationDirective(LocationConfig& location); // location içi direktif
 
    // Yardımcı: "1m" / "10k" / "1024" → byte sayısına çevir
    size_t parseSize(const std::string& val) const;
 
    // Yardımcı: int → string (C++98'de to_string yok)
    std::string intToStr(int n) const;
};
 
#endif
 
