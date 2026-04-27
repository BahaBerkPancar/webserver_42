
#ifndef CONFIG_HPP
#define CONFIG_HPP
 
#include <string>
#include <vector>
#include <map>
#include <utility>  // std::pair

// ===== Token Type Enum =====
 
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
 
// ===== Location Configuration =====
 
struct LocationConfig {
    std::string              path;           // URL path: "/", "/api", etc.
    std::vector<std::string> methods;        // Allowed HTTP methods
    std::string              root;           // Document root directory
    std::string              index;          // Default file in directory
    bool                     autoindex;      // Enable directory listing
    std::string              redirect;       // HTTP redirect (empty if none)
    std::string              upload_dir;     // Upload directory (empty if disabled)
    std::string              cgi_extension;  // CGI file extension (e.g. .php)
    std::string              cgi_path;       // CGI interpreter path (empty if disabled)
 
    LocationConfig()
        : autoindex(false) {}
};
 

// ===== Server Configuration =====
 
struct ServerConfig {
    std::vector<std::pair<std::string, int> > listen;
    std::map<int, std::string> error_pages;
    std::vector<LocationConfig> locations;
    size_t client_max_body_size;
 
    ServerConfig()
        : client_max_body_size(1048576) {}
};
 
// ===== Global Configuration =====
 
struct Config {
    std::vector<ServerConfig> servers;
};

#endif
