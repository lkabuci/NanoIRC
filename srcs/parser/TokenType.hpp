#pragma once

#ifndef __TOKEN_TYPE_HPP__
#define __TOKEN_TYPE_HPP__

namespace TYPES {
enum TokenType {
    // commands
    PASS,
    NICK,
    USER,
    KICK,
    INVITE,
    TOPIC,
    MODE,

    // separators
    CRLF,
    CR,
    LF,
    AMPERSAND,
    HASH,
    COMMA,
    PLUS,
    MINUS,
    SPACE,
    SEMICOLON,

    LETTER,
    END
};
}

#endif