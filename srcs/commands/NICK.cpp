#include "NICK.hpp"

NICK::NICK() {}

NICK::NICK(const NICK& nick) : _nick(nick._nick) {}

NICK::~NICK() {}

NICK& NICK::operator=(const NICK& nick) {
    if (this == &nick)
        return *this;
    _nick = nick._nick;
    return *this;
}

void NICK::execute(const std::string& first, const std::string& parameters) {
    (void)first;
    if (Parser::is_at_end()) {
        ErrorReplies::reply(2, "me", ERROR_CODES::ERR_NEEDMOREPARAMS,
                            "chiwa7d");
        throw std::exception();
    }
    _nick = Parser::advance().lexeme();
    if (!Parser::is_at_end()) {
        ErrorReplies::reply(2, "me", ERROR_CODES::ERR_NEEDMOREPARAMS,
                            "chiwa7d");
        throw std::exception();
    }
}

const std::string& NICK::get() const {
    return _nick;
}
