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
    if (_notEnoughParams(client, parameters) || !_userSetPassword(client))
        return;
    if (!_setNickname(client, parameters[0]) || _nicknameAlreadyExists(client))
        return;
    client->getUserInfo().setNickname(_nick);
    if (!ClientList::exist(_nick))
        ClientList::add(client);
    if (client->getUserInfo().isRegistered())
        _welcomeUser(client);
}

bool NICK::_setNickname(Client* client, const std::string& param) {
    if (!Parser::nick(param, _nick)) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_ERRONEUSNICKNAME,
                     client->getUserInfo().getNickname(), "");
        return false;
    }
    return true;
    // try {
    //     _nick = Utils::getNickname(param);
    // } catch (const std::exception& e) {
    //     Reply::error(client->getSockfd(), ERROR_CODES::ERR_ERRONEUSNICKNAME,
    //                  client->getUserInfo().getNickname(), "");
    //     return false;
    // }
    // return true;
}

bool NICK::_nicknameAlreadyExists(Client* client) {
    if (ClientList::exist(_nick) &&
        _nick != client->getUserInfo().getNickname()) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_NICKNAMEINUSE, _nick,
                     "");
        return true;
    }
    return false;
}

bool NICK::_notEnoughParams(Client*                         client,
                            const std::vector<std::string>& parameters) {
    if (parameters.empty() || parameters.size() > 2) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_NEEDMOREPARAMS,
                     client->getUserInfo().getNickname(), "NICK");
        return true;
    }
    return false;
}

bool NICK::_userSetPassword(Client* client) {
    if (client->getUserInfo().isSet(UserInfo::PASSWORD_SET))
        return true;
    Reply::error(client->getSockfd(), ERROR_CODES::ERR_NOTREGISTERED,
                 client->getUserInfo().getNickname(), "");
    return false;
}

void NICK::_welcomeUser(Client* client) {
    Reply::rpl_welcome(client->getSockfd(), client->getUserInfo().getNickname(),
                       client->getUserInfo().getUsername());
}
