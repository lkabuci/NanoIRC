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
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_NICKNAMEINUSE, "");
    if (!client->getUserInfo().isSet(UserInfo::PASSWORD_SET))
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_NOTREGISTERED, "");
    _nick = parameters[0];
    if (_isNicknameCollision(client)) {
        _removeInstances();
        _sendErrorReply(client);
    }
    client->getUserInfo().setNickname(_nick);
    if (!ClientList::exist(_nick))
        ClientList::add(client);
    _sendSuccessReply(client);
}

void NICK::_sendErrorReply(Client* client) {
    Reply::error(client->getSockfd(), ERROR_CODES::ERR_NICKNAMEINUSE,
                 "*" + _nick);
}

void NICK::_sendSuccessReply(Client* client) {
    std::string user = client->getUserInfo().getUsername();

    if (user.empty())
        user = _nick;

    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      " 001 " + client->getUserInfo().getNickname() + " " +
                      Reply::successReply[SUCCESS_CODES::RPL_WELCOME] + "\r\n";

    send(client->getSockfd(), msg.c_str(), msg.length(), 0);
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
