#include "Lexer.hpp"

std::map<std::string, TYPES::TokenType> Lexer::_commands(_init());

Lexer::Lexer() : _message(""), _start(0), _current(0), _token() {}

Lexer::Lexer(const std::string& message)
    : _message(message), _start(0), _current(0), _length(_message.length()),
      _token() {}

Lexer::Lexer(const Lexer& lexer)
    : _message(lexer._message), _start(lexer._start), _current(lexer._current),
      _length(lexer._length), _token(lexer._token) {}

Lexer::~Lexer() {}

Lexer& Lexer::operator=(const Lexer& lexer) {
    if (this == &lexer)
        return *this;
    _message = lexer._message;
    _start = lexer._start;
    _current = lexer._current;
    _length = lexer._length;
    _token = lexer._token;
    return *this;
}

Token Lexer::getNextToken() {
    char c;

    while (!_isAtEnd()) {
        _start = _current;
        switch (c = _advance()) {
        case '\0':
            break;
        case '#':
            return Token(TYPES::HASH, "#");
        case ',':
            return Token(TYPES::COMMA, ",");
        case '+':
            return Token(TYPES::PLUS, "+");
        case '-':
            return Token(TYPES::MINUS, "-");
        case '\r':
            if (_match('\n'))
                return Token(TYPES::CRLF, "\r\n");
            return Token(TYPES::CR, "\r");
        case '\n':
            return Token(TYPES::LF, "\n");
        case ' ':
            return Token(TYPES::SPACE, " ");
        case ':':
            return Token(TYPES::SEMICOLON, ":");
        default:
            _letter();
            return _token;
        }
    }
    return Token(TYPES::END, "");
}

void Lexer::_letter() {
    while (!_isAtEnd() && !_isSepatator())
        _advance();
    std::string      lexeme = _message.substr(_start, _current - _start);
    TYPES::TokenType type;

    if (_commands.find(lexeme) != _commands.end())
        type = _commands[lexeme];
    else
        type = TYPES::LETTER;
    _token = Token(type, lexeme);
}

char Lexer::_advance() {
    if (_isAtEnd())
        return '\0';
    return _message.at(_current++);
}

bool Lexer::_match(char expected) {
    if (_isAtEnd() || _message.at(_current) != expected)
        return false;
    ++_current;
    return true;
}

bool Lexer::_isSepatator() {
    switch (_message.at(_current)) {
    case '&':
    case '#':
    case ',':
    case '+':
    case '-':
    case '\r':
    case '\n':
    case ' ':
    case ':':
        return true;
    default:
        break;
    }
    return false;
}

bool Lexer::_isAtEnd() const {
    return _current >= _length;
}

std::map<std::string, TYPES::TokenType> Lexer::_init() {
    std::map<std::string, TYPES::TokenType> m;

    m["PASS"] = TYPES::PASS;
    m["NICK"] = TYPES::NICK;
    m["USER"] = TYPES::USER;
    m["JOIN"] = TYPES::JOIN;
    m["KICK"] = TYPES::KICK;
    m["INVITE"] = TYPES::INVITE;
    m["TOPIC"] = TYPES::TOPIC;
    m["MODE"] = TYPES::MODE;
    m["PRIVMSG"] = TYPES::PRIVMSG;
    m["NOTICE"] = TYPES::NOTICE;
    return m;
}
