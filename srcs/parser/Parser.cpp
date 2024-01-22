#include "Parser.hpp"

Lexer Parser::_lexer;
Token Parser::_token;
Token Parser::_prev;

void Parser::init(const std::string& source) {
    _lexer = Lexer(source);
    _token = _lexer.getNextToken();
    _prev = _token;
}

void Parser::consume(const TYPES::TokenType& type, const char* message) {
    if (!match(type))
        throw std::runtime_error(message);
}

bool Parser::match(const TYPES::TokenType& type) {
    if (isAtEnd() || !check(type))
        return false;
    advance();
    return true;
}

const Token& Parser::advance() {
    if (isAtEnd())
        return _token;
    _prev = _token;
    _token = _lexer.getNextToken();
    return _prev;
}

const Token& Parser::previous() {
    return _prev;
}

const Token& Parser::peek() {
    return _token;
}

const Token& Parser::end() {
    while (!isAtEnd())
        advance();
    return _prev;
}

bool Parser::check(const TYPES::TokenType& type) {
    return _token.type() == type;
}

bool Parser::isAtEnd() {
    return _token.type() == TYPES::END;
}

bool Parser::skipSpaces() {
    if (!check(TYPES::SPACE))
        return false;
    while (match(TYPES::SPACE))
        ;
    return true;
}

bool Parser::channel(const std::string& source, std::string& target) {
    size_t i = 0;

    if (source[i] != '#')
        return false;
    while (_chstring(source[++i]))
        ;
    target = source.substr(0, i);
    return target.length() == source.length();
}

bool Parser::name(const std::string& source, std::string& target) {
    size_t i = 0;

    if (!std::isalpha(source[i]))
        return false;
    while (std::isalnum(source[i]) || _special(source[i]))
        i++;
    target = source.substr(0, i);
    return target.length() == source.length();
}

bool Parser::_nonwhite(char c) {
    return c != '\0' && c != ' ' && c != '\r' && c != '\n';
}

bool Parser::_special(char c) {
    return c == '-' || c == '[' || c == ']' || c == '\\' || c == '`' ||
           c == '^' || c == '{' || c == '}';
}

bool Parser::_chstring(char c) {
    return c != '\0' && c != ' ' && c != '\r' && c != '\n' && c != ',';
}
