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

void NICK::execute(Client* client, const std::vector<std::string>& parameters) {
    if (parameters.empty() || parameters.size() > 2)
        throw std::runtime_error("431 ERR_NONICKNAMEGIVEN:No nickname given.");
    _nick = parameters[0];
    if (Clients::exist(_nick)) {
        throw std::runtime_error(
            "433 ERR_NICKNAMEINUSE:Nickname is already in use.");
    }
    client->getUserInfo().setNickname(_nick);
}

const std::string& NICK::getNickname() const {
    return _nick;
}
