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

void NICK::execute(const std::vector<std::string>& parameters) {
    if (parameters.size() > 2)
        throw std::runtime_error("USER <nickname> [ <hopcount> ]");
    _nick = parameters[0];
}

const std::string& NICK::getNickname() const {
    return _nick;
}
