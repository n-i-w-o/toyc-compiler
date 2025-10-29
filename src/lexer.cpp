#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>

enum class TokenType {
    INT, VOID, IF, ELSE, WHILE, BREAK, CONTINUE, RETURN,
    IDENT, INTCONST,
    PLUS, MINUS, MULTIPLY, DIVIDE, MOD,
    ASSIGN, EQ, NE, LT, GT, LE, GE,
    AND, OR, NOT,
    LPAREN, RPAREN, LBRACE, RBRACE, COMMA, SEMICOLON
};

struct Token {
    int line;
    TokenType type;
    std::string value;
};

class Lexer {
private:
    std::string input;
    int pos;
    int line;
    char current_char;
    
    std::unordered_map<std::string, TokenType> keywords = {
        {"int", TokenType::INT}, {"void", TokenType::VOID}, {"if", TokenType::IF},
        {"else", TokenType::ELSE}, {"while", TokenType::WHILE}, {"break", TokenType::BREAK},
        {"continue", TokenType::CONTINUE}, {"return", TokenType::RETURN}
    };
    
    std::unordered_map<TokenType, std::string> token_names = {
        {TokenType::INT, "'int'"}, {TokenType::VOID, "'void'"}, {TokenType::IF, "'if'"},
        {TokenType::ELSE, "'else'"}, {TokenType::WHILE, "'while'"}, {TokenType::BREAK, "'break'"},
        {TokenType::CONTINUE, "'continue'"}, {TokenType::RETURN, "'return'"},
        {TokenType::IDENT, "Ident"}, {TokenType::INTCONST, "IntConst"},
        {TokenType::PLUS, "'+'"}, {TokenType::MINUS, "'-'"}, {TokenType::MULTIPLY, "'*'"},
        {TokenType::DIVIDE, "'/'"}, {TokenType::MOD, "'%'"}, {TokenType::ASSIGN, "'='"},
        {TokenType::EQ, "'=='"}, {TokenType::NE, "'!='"}, {TokenType::LT, "'<'"},
        {TokenType::GT, "'>'"}, {TokenType::LE, "'<='"}, {TokenType::GE, "'>='"},
        {TokenType::AND, "'&&'"}, {TokenType::OR, "'||'"}, {TokenType::NOT, "'!'"},
        {TokenType::LPAREN, "'('"}, {TokenType::RPAREN, "')'"}, {TokenType::LBRACE, "'{'"},
        {TokenType::RBRACE, "'}'"}, {TokenType::COMMA, "','"}, {TokenType::SEMICOLON, "';'"}
    };

    void advance() {
        pos++;
        if (pos < input.length()) {
            current_char = input[pos];
        } else {
            current_char = '\0';
        }
    }

    void skip_whitespace() {
        while (current_char != '\0' && std::isspace(current_char)) {
            if (current_char == '\n') line++;
            advance();
        }
    }

    void skip_comment() {
        if (current_char == '/') {
            advance();
            if (current_char == '/') {
                while (current_char != '\0' && current_char != '\n') advance();
                if (current_char == '\n') { line++; advance(); }
            } else if (current_char == '*') {
                advance();
                while (current_char != '\0') {
                    if (current_char == '*') {
                        advance();
                        if (current_char == '/') { advance(); break; }
                    } else {
                        if (current_char == '\n') line++;
                        advance();
                    }
                }
            } else {
                pos--;
                current_char = '/';
            }
        }
    }

    Token number() {
        std::string result;
        bool has_digit = false;
        
        if (current_char == '-') {
            Token token;
            token.line = line;
            token.type = TokenType::MINUS;
            token.value = "-";
            return token;
        }
        
        while (current_char != '\0' && std::isdigit(current_char)) {
            result += current_char;
            has_digit = true;
            advance();
        }
        
        if (!has_digit) result = "0";
        
        Token token;
        token.line = line;
        token.type = TokenType::INTCONST;
        token.value = result;
        return token;
    }

    Token identifier() {
        std::string result;
        
        if (current_char == '_' || std::isalpha(current_char)) {
            result += current_char;
            advance();
        }
        
        while (current_char != '\0' && (current_char == '_' || std::isalnum(current_char))) {
            result += current_char;
            advance();
        }
        
        Token token;
        token.line = line;
        
        if (keywords.find(result) != keywords.end()) {
            token.type = keywords[result];
            token.value = result;
        } else {
            token.type = TokenType::IDENT;
            token.value = result;
        }
        
        return token;
    }

public:
    Lexer(const std::string& text) : input(text), pos(0), line(1) {
        if (!input.empty()) current_char = input[0];
        else current_char = '\0';
    }

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        
        while (current_char != '\0') {
            if (std::isspace(current_char)) {
                skip_whitespace();
                continue;
            }
            
            if (current_char == '/') {
                int old_pos = pos, old_line = line;
                skip_comment();
                if (pos == old_pos && line == old_line) {
                } else continue;
            }
            
            Token token;
            token.line = line;
            
            if (current_char == '_' || std::isalpha(current_char)) {
                token = identifier();
                tokens.push_back(token);
                continue;
            }
            
            if (std::isdigit(current_char)) {
                token = number();
                tokens.push_back(token);
                continue;
            }
            
            switch (current_char) {
                case '+': token.type = TokenType::PLUS; token.value = "+"; advance(); break;
                case '-': token.type = TokenType::MINUS; token.value = "-"; advance(); break;
                case '*': token.type = TokenType::MULTIPLY; token.value = "*"; advance(); break;
                case '/': token.type = TokenType::DIVIDE; token.value = "/"; advance(); break;
                case '%': token.type = TokenType::MOD; token.value = "%"; advance(); break;
                case '=':
                    advance();
                    if (current_char == '=') { token.type = TokenType::EQ; token.value = "=="; advance(); }
                    else { token.type = TokenType::ASSIGN; token.value = "="; } break;
                case '!':
                    advance();
                    if (current_char == '=') { token.type = TokenType::NE; token.value = "!="; advance(); }
                    else { token.type = TokenType::NOT; token.value = "!"; } break;
                case '<':
                    advance();
                    if (current_char == '=') { token.type = TokenType::LE; token.value = "<="; advance(); }
                    else { token.type = TokenType::LT; token.value = "<"; } break;
                case '>':
                    advance();
                    if (current_char == '=') { token.type = TokenType::GE; token.value = ">="; advance(); }
                    else { token.type = TokenType::GT; token.value = ">"; } break;
                case '&':
                    advance();
                    if (current_char == '&') { token.type = TokenType::AND; token.value = "&&"; advance(); }
                    else { token.type = TokenType::AND; token.value = "&"; } break;
                case '|':
                    advance();
                    if (current_char == '|') { token.type = TokenType::OR; token.value = "||"; advance(); }
                    else { token.type = TokenType::OR; token.value = "|"; } break;
                case '(': token.type = TokenType::LPAREN; token.value = "("; advance(); break;
                case ')': token.type = TokenType::RPAREN; token.value = ")"; advance(); break;
                case '{': token.type = TokenType::LBRACE; token.value = "{"; advance(); break;
                case '}': token.type = TokenType::RBRACE; token.value = "}"; advance(); break;
                case ',': token.type = TokenType::COMMA; token.value = ","; advance(); break;
                case ';': token.type = TokenType::SEMICOLON; token.value = ";"; advance(); break;
                default: advance(); continue;
            }
            
            tokens.push_back(token);
        }
        
        return tokens;
    }
    
    std::string get_token_name(TokenType type) { return token_names[type]; }
};

int main() {
    std::string input, line;
    while (std::getline(std::cin, line)) input += line + "\n";
    Lexer lexer(input);
    std::vector<Token> tokens = lexer.tokenize();
    for (size_t i = 0; i < tokens.size(); i++) {
        std::cout << i << ":" << lexer.get_token_name(tokens[i].type) << ":\"" 
                  << tokens[i].value << "\"" << std::endl;
    }
    return 0;
}