#pragma once

#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include "../../include/IRC.h"

class Token {
  public:
    Token();
    Token(const TYPES::TokenType& type, const std::string& lexeme);
    Token(const Token& token);
    ~Token();

    Token& operator=(const Token& token);

    const TYPES::TokenType& type() const;
    const std::string&      lexeme() const;

    void setRype(const TYPES::TokenType& type);
    void setLexeme(const std::string& lexeme);

  private:
    TYPES::TokenType _type;
    std::string      _lexeme;
};

#endif
