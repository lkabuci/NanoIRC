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
    if (_notEnoughParams(client, parameters))
        return;
    if (!_userSetPassword(client))
        return;
    _nick = parameters[0];
    if (_isNicknameCollision(client)) {
        _removeInstances();
        _sendErrNicknameCollision(client);
        return;
    }
    client->getUserInfo().setNickname(_nick);
    if (!ClientList::exist(_nick))
        ClientList::add(client);
    if (client->getUserInfo().isRegistered())
        _welcomeUser(client);
}

void NICK::_sendErrNicknameCollision(Client* client) {
    Reply::error(client->getSockfd(), ERROR_CODES::ERR_NICKCOLLISION,
                 client->getUserInfo().getNickname(),
                 client->getUserInfo().getUsername());
}

bool NICK::_notEnoughParams(Client*                         client,
                            const std::vector<std::string>& parameters) {
    if (parameters.empty() || parameters.size() > 2) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_NEEDMOREPARAMS,
                     client->getUserInfo().getNickname(), "NICK");
        return false;
    }
    return true;
}

bool NICK::_userSetPassword(Client* client) {
    if (client->getUserInfo().isSet(UserInfo::PASSWORD_SET))
        return true;
    Reply::error(client->getSockfd(), ERROR_CODES::ERR_NOTREGISTERED,
                 client->getUserInfo().getNickname(), "");
    return false;
}

bool NICK::_isNicknameCollision(Client* client) {
    return ClientList::exist(_nick) &&
           _nick != client->getUserInfo().getNickname();
}

void NICK::_removeInstances() {
    while (ClientList::exist(_nick))
        ClientList::remove(_nick);
}

void NICK::_welcomeUser(Client* client) {
    Reply::rpl_welcome(client->getSockfd(), client->getUserInfo().getNickname(),
                       client->getUserInfo().getUsername());
}
