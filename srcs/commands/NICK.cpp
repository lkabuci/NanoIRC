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
    if (!client->getUserInfo().isSet(UserInfo::PASSWORD_SET))
        throw std::runtime_error("User must set a password first.");
    _nick = parameters[0];
    if (_isNicknameCollision(client)) {
        _removeInstances();
        throw std::runtime_error(
            "433 ERR_NICKNAMEINUSE:Nickname is already in use.");
    }
    client->getUserInfo().setNickname(_nick);
    if (!ClientList::exist(_nick))
        ClientList::add(client);
}

void NICK::_removeInstances() {
    while (ClientList::exist(_nick))
        ClientList::remove(_nick);
}

bool NICK::_isNicknameCollision(Client* client) {
    return ClientList::exist(_nick) &&
           _nick != client->getUserInfo().getNickname();
}

const std::string& NICK::getNickname() const {
    return _nick;
}
