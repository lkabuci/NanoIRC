#pragma once

#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include "ErrorReplies.hpp"
#include "Lexer.hpp"

class Parser {
  public:
    static void init(const std::string& source);
    static void consume(const TYPES::TokenType& type, ERROR_CODES::Codes code);
    static bool match(const TYPES::TokenType& type);
    static const Token& advance();
    static const Token& previous();
    static const Token& peek();
    static bool         check(const TYPES::TokenType& type);
    static bool         is_at_end();
    static void         skip_spaces();

  private:
    Parser();

    static Lexer _lexer;
    static Token _token;
    static Token _prev;
};

#endif