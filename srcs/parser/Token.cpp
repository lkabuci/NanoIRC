#include "Token.hpp"

Token::Token() : _type(TYPES::END), _lexeme("") {}

Token::Token(const TYPES::TokenType& type, const std::string& lexeme)
    : _type(type)
    , _lexeme(lexeme)
{
}

Token::Token(const Token& token) : _type(token._type), _lexeme(token._lexeme) {}

Token::~Token() {}

Token&  Token::operator=(const Token& token) {
    if (this == &token)
        return *this;
    _type = token._type;
    _lexeme = token._lexeme;
    return *this;
}

const TYPES::TokenType&    Token::type() const { return _type; }

const std::string&  Token::lexeme() const { return _lexeme; }

void    Token::set_type(const TYPES::TokenType& type) {
    _type = type;
}

void    Token::set_lexeme(const std::string& lexeme) {
    _lexeme = lexeme;
}
