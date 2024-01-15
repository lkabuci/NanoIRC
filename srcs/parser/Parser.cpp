#include "Parser.hpp"

Lexer Parser::_lexer;
Token Parser::_token;
Token Parser::_prev;

void Parser::init(const std::string& source) {
    _lexer = Lexer(source);
    _token = _lexer.get_next_token();
    _prev = _token;
}

void Parser::consume(const TYPES::TokenType& type, ERROR_CODES::Codes code) {
    if (!match(type)) {
        ErrorReplies::reply(2, "localhost", code, "c");
        throw std::exception();
    }
}

bool Parser::match(const TYPES::TokenType& type) {
    if (is_at_end() || !check(type))
        return false;
    advance();
    return true;
}

const Token& Parser::advance() {
    if (is_at_end())
        return _token;
    _prev = _token;
    _token = _lexer.get_next_token();
    return _prev;
}

const Token& Parser::previous() {
    return _prev;
}

const Token& Parser::peek() {
    return _token;
}

bool Parser::check(const TYPES::TokenType& type) {
    return _token.type() == type;
}

bool Parser::is_at_end() {
    return _token.type() == TYPES::END;
}

void Parser::skip_spaces() {
    while (check(TYPES::SPACE))
        advance();
}
