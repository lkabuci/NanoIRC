#pragma once

#ifndef __LEXER_HPP__
#define __LEXER_HPP__

#include "Token.hpp"

class Lexer {
  public:
    Lexer();
    Lexer(const std::string& message);
    Lexer(const Lexer& lexer);
    ~Lexer();

    Lexer& operator=(const Lexer& lexer);

    Token get_next_token();

    static std::map<TYPES::TokenType, std::string> values;

  private:
    static std::map<std::string, TYPES::TokenType> _commands;
    static std::map<std::string, TYPES::TokenType> _init();
    static std::map<TYPES::TokenType, std::string> _init_values();

    std::string _message;
    size_t      _start;
    size_t      _current;
    size_t      _length;
    Token       _token;

    void _letter();
    char _advance();
    bool _match(char expected);
    bool _is_sepatator();
    bool _is_at_end() const;
};

#endif