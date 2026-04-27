
#ifndef CONFIG_HPP
#define CONFIG_HPP
 
#include <string>
#include <vector>
#include <map>
#include <utility>  // std::pair

// ===== Token =====
 
enum TokenType {
    TOKEN_WORD,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_SEMICOLON,
    TOKEN_EOF
};
 
struct Token {
    TokenType   type;
    std::string value;
    int         line;
 
    Token(TokenType t, const std::string& v, int l)
        : type(t), value(v), line(l) {}
};
 
// ===== LocationConfig =====
 
struct LocationConfig {
    std::string              path;           // "/", "/api", "/upload"
    std::vector<std::string> methods;        // ["GET", "POST", "DELETE"]
    std::string              root;           // "/var/www/html"
    std::string              index;          // "index.html"
    bool                     autoindex;      // true / false
    std::string              redirect;       // "301 /yeni/yol" — boşsa yok
    std::string              upload_dir;     // "/tmp/uploads"  — boşsa yok
    std::string              cgi_extension;  // ".php", ".py"   — boşsa yok
    std::string              cgi_path;       // "/usr/bin/php"  — boşsa yok
 
    LocationConfig()
        : autoindex(false) {}
};
 

// ===== ServerConfig =====
 
struct ServerConfig {
    std::vector<std::pair<std::string, int> > listen;
    // [("0.0.0.0", 8080), ("127.0.0.1", 9090)]
    // C++98: "> >" arasında boşluk zorunlu
 
    std::map<int, std::string> error_pages;
    // { 404 -> "/errors/404.html", 500 -> "/errors/50x.html" }
 
    size_t client_max_body_size;
    // byte cinsinden — "1m" -> 1048576
 
    std::vector<LocationConfig> locations;
 
    ServerConfig()
        : client_max_body_size(1048576) {}  // varsayılan: 1MB
};
 
// ===== Config =====
 
struct Config {
    std::vector<ServerConfig> servers;
};

#endif
