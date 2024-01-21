#pragma once

#ifndef __TOKEN_TYPE_HPP__
#define __TOKEN_TYPE_HPP__

namespace TYPES {
enum TokenType {
    // commands
    PASS,
    NICK,
    USER,
    JOIN,
    KICK,
    INVITE,
    TOPIC,
    MODE,
    PRIVMSG,
    NOTICE,
    QUIT,

    // separators
    CRLF,
    CR,
    LF,
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
