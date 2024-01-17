#pragma once

#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include "Lexer.hpp"

class Parser {
  public:
    static void init(const std::string& source);
    static void consume(const TYPES::TokenType& type, const char* message);
    static bool match(const TYPES::TokenType& type);
    static const Token& advance();
    static const Token& previous();
    static const Token& peek();
    static bool         check(const TYPES::TokenType& type);
    static bool         isAtEnd();
    static bool         skipSpaces();

  private:
    Parser();

    static Lexer _lexer;
    static Token _token;
    static Token _prev;
};

#endif
