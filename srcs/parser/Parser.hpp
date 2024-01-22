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
    static const Token& end();
    static bool         check(const TYPES::TokenType& type);
    static bool         isAtEnd();
    static bool         skipSpaces();

    // special for parsing targets
    static bool channel(const std::string& source, std::string& target);
    static bool name(const std::string& source, std::string& target);
    // static bool user(const std::string& source, std::string& target);

  private:
    Parser();

    static Lexer _lexer;
    static Token _token;
    static Token _prev;

    static bool _chstring(char c);
    static bool _special(char c);
    static bool _nonwhite(char c);
};

#endif
