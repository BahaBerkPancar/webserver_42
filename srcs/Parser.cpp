#include "Parser.hpp"
#include <sstream>
#include <cstdlib>

Parser::Parser(const std::vector<Token>& tokens)
    : _tokens(tokens), _pos(0) {}

Token Parser::peek() const {
    if (_pos >= _tokens.size())
        return Token(TOKEN_EOF, "", 0);
    return _tokens[_pos];
}

Token Parser::advance() {
    Token current = peek();
    if (_pos < _tokens.size())
        _pos++;
    return current;
}

Token Parser::expect(TokenType type) {
    Token current = peek();
    if (current.type != type) {
        std::string msg = "Parser: unexpected token '";
        msg += current.value;
        msg += "' at line ";
        msg += intToStr(current.line);
        msg += " (expected ";
        
        switch(type) {
            case TOKEN_LBRACE: msg += "{"; break;
            case TOKEN_RBRACE: msg += "}"; break;
            case TOKEN_SEMICOLON: msg += ";"; break;
            case TOKEN_WORD: msg += "word"; break;
            case TOKEN_EOF: msg += "EOF"; break;
        }
        msg += ")";
        
        throw std::runtime_error(msg);
    }
    return advance();
}

Config Parser::parse() {
    Config config;

    while (peek().type != TOKEN_EOF) {
        Token word = peek();
        
        // Skip unexpected tokens
        if (word.type != TOKEN_WORD) {
            advance();
            continue;
        }

        // Parse server block
        if (word.value == "server") {
            advance();  // Skip "server"
            config.servers.push_back(parseServer());
        } else {
            advance();  // Skip unknown directive
        }
    }

    return config;
}

ServerConfig Parser::parseServer() {
    ServerConfig server;

    expect(TOKEN_LBRACE);  // Expect "{"

    while (peek().type != TOKEN_RBRACE && peek().type != TOKEN_EOF) {
        parseServerDirective(server);
    }

    expect(TOKEN_RBRACE);  // Expect "}"

    return server;
}

void Parser::parseServerDirective(ServerConfig& server) {
    Token directive = peek();

    if (directive.type != TOKEN_WORD) {
        advance();
        return;
    }

    std::string dir = directive.value;
    advance();  // Skip directive name

    if (dir == "listen") {
        // Parse: listen <host>:<port>;
        Token hostPort = expect(TOKEN_WORD);
        expect(TOKEN_SEMICOLON);

        // Split by ":"
        std::string host = hostPort.value;
        int port = 8080;  // Default port
        size_t colonPos = hostPort.value.find(':');

        if (colonPos != std::string::npos) {
            host = hostPort.value.substr(0, colonPos);
            std::string portStr = hostPort.value.substr(colonPos + 1);
            port = std::atoi(portStr.c_str());
        }

        server.listen.push_back(std::make_pair(host, port));
    }
    else if (dir == "client_max_body_size") {
        // Parse: client_max_body_size <size>;
        Token sizeToken = expect(TOKEN_WORD);
        expect(TOKEN_SEMICOLON);
        server.client_max_body_size = parseSize(sizeToken.value);
    }
    else if (dir == "error_page") {
        // Parse: error_page <code> <path>;
        Token codeToken = expect(TOKEN_WORD);
        Token pathToken = expect(TOKEN_WORD);
        expect(TOKEN_SEMICOLON);

        int code = std::atoi(codeToken.value.c_str());
        server.error_pages[code] = pathToken.value;
    }
    else if (dir == "location") {
        // Parse: location <path> { ... }
        Token pathToken = expect(TOKEN_WORD);
        LocationConfig location = parseLocation();
        location.path = pathToken.value;
        server.locations.push_back(location);
    }
    else {
        // Skip unknown directive until semicolon
        while (peek().type != TOKEN_SEMICOLON && peek().type != TOKEN_EOF)
            advance();
        if (peek().type == TOKEN_SEMICOLON)
            advance();
    }
}

LocationConfig Parser::parseLocation() {
    LocationConfig location;

    expect(TOKEN_LBRACE);  // Expect "{"

    while (peek().type != TOKEN_RBRACE && peek().type != TOKEN_EOF) {
        parseLocationDirective(location);
    }

    expect(TOKEN_RBRACE);  // Expect "}"

    return location;
}

void Parser::parseLocationDirective(LocationConfig& location) {
    Token directive = peek();

    if (directive.type != TOKEN_WORD) {
        advance();
        return;
    }

    std::string dir = directive.value;
    advance();  // Skip directive name

    if (dir == "root") {
        // Parse: root <path>;
        Token pathToken = expect(TOKEN_WORD);
        expect(TOKEN_SEMICOLON);
        location.root = pathToken.value;
    }
    else if (dir == "index") {
        // Parse: index <file>;
        Token fileToken = expect(TOKEN_WORD);
        expect(TOKEN_SEMICOLON);
        location.index = fileToken.value;
    }
    else if (dir == "methods") {
        // Parse: methods <method1> <method2> ...;
        while (peek().type == TOKEN_WORD) {
            Token method = advance();
            location.methods.push_back(method.value);
        }
        expect(TOKEN_SEMICOLON);
    }
    else if (dir == "autoindex") {
        // Parse: autoindex on|off;
        Token onOff = expect(TOKEN_WORD);
        expect(TOKEN_SEMICOLON);
        location.autoindex = (onOff.value == "on");
    }
    else if (dir == "redirect") {
        // Parse: redirect <code> <path>;
        Token codeToken = expect(TOKEN_WORD);
        Token pathToken = expect(TOKEN_WORD);
        expect(TOKEN_SEMICOLON);
        location.redirect = codeToken.value + " " + pathToken.value;
    }
    else if (dir == "upload_dir") {
        // Parse: upload_dir <path>;
        Token pathToken = expect(TOKEN_WORD);
        expect(TOKEN_SEMICOLON);
        location.upload_dir = pathToken.value;
    }
    else if (dir == "cgi_extension") {
        // Parse: cgi_extension <ext>;
        Token extToken = expect(TOKEN_WORD);
        expect(TOKEN_SEMICOLON);
        location.cgi_extension = extToken.value;
    }
    else if (dir == "cgi_path") {
        // Parse: cgi_path <path>;
        Token pathToken = expect(TOKEN_WORD);
        expect(TOKEN_SEMICOLON);
        location.cgi_path = pathToken.value;
    }
    else {
        // Skip unknown directive until semicolon
        while (peek().type != TOKEN_SEMICOLON && peek().type != TOKEN_EOF)
            advance();
        if (peek().type == TOKEN_SEMICOLON)
            advance();
    }
}

size_t Parser::parseSize(const std::string& val) const {
    size_t size = 0;

    // Check for suffixes: K, M, G
    if (!val.empty()) {
        char suffix = val[val.length() - 1];
        std::string numPart = val;

        if (suffix == 'K' || suffix == 'k') {
            numPart = val.substr(0, val.length() - 1);
            size = std::atol(numPart.c_str()) * 1024;
        }
        else if (suffix == 'M' || suffix == 'm') {
            numPart = val.substr(0, val.length() - 1);
            size = std::atol(numPart.c_str()) * 1024 * 1024;
        }
        else if (suffix == 'G' || suffix == 'g') {
            numPart = val.substr(0, val.length() - 1);
            size = std::atol(numPart.c_str()) * 1024 * 1024 * 1024;
        }
        else {
            // No suffix, just parse as number
            size = std::atol(val.c_str());
        }
    }

    return size;
}

std::string Parser::intToStr(int n) const {
    std::ostringstream oss;
    oss << n;
    return oss.str();
}
