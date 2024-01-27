#include "NICK.hpp"

NICK::NICK() {}

NICK::~NICK() {}

void NICK::execute(Client* client, const std::vector<std::string>& parameters) {
    if (_notEnoughParams(client, parameters) || !_userSetPassword(client))
        return;
    if (_nicknameAlreadyInUse(client))
        return;
    Parser::name(parameters[0], _nick);
    client->getUserInfo().setNickname(_nick);
    if (!ClientList::exist(_nick))
        ClientList::add(client);
    if (client->getUserInfo().isRegistered())
        _welcome(client);
}

bool NICK::_nicknameAlreadyInUse(Client* client) {
    if (ClientList::exist(_nick) &&
        _nick != client->getUserInfo().getNickname()) {
        //: euroserv.fr.quakenet.org 433 * n1 :Nickname is already in use.
        _errNicknameAlreadyInUse(client);
        return true;
    }
    return false;
}

bool NICK::_notEnoughParams(Client*                         client,
                            const std::vector<std::string>& parameters) {
    if (parameters.empty()) {
        //: underworld2.no.quakenet.org 431 i1 :No nickname given
        _errNoNicknameGiven(client);
        return true;
    }
    return false;
}

bool NICK::_userSetPassword(Client* client) {
    if (client->getUserInfo().isSet(UserInfo::PASSWORD_SET))
        return true;
    std::string reply =
        std::string(":") + Reactor::getInstance().getServerIp() + " 451 ";

    if (client->getUserInfo().getNickname().empty()) {
        //: atw.hu.quakenet.org 451 *  :Register first.
        reply.append("*  ");
    } else {
        //: atw.hu.quakenet.org 451 i1 i1 :Register first.
        reply.append(client->getUserInfo().getNickname() + " " +
                     client->getUserInfo().getUsername());
    }
    reply.append(" :You have not registered\r\n");
    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
    return false;
}

void NICK::_welcome(Client* client) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      " 001 " + _nick + " :sf ghayerha, " + _nick + "!" +
                      client->getUserInfo().getUsername() + "@" +
                      Reactor::getInstance().getServerIp() + CR_LF;

    send(client->getSockfd(), msg.c_str(), msg.length(), 0);
}

void NICK::_errNoNicknameGiven(Client* client) {
    std::string nick = _nick.empty() ? "*" : _nick;
    std::string reply = std::string(":") +
                        Reactor::getInstance().getServerIp() + " 431 " + nick +
                        " :No nickname given\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void NICK::_errNicknameAlreadyInUse(Client* client) {
    std::string reply =
        std::string(":") + Reactor::getInstance().getServerIp() + " 433 ";

    if (client->getUserInfo().getNickname().empty()) {
        reply.append("*");
    } else {
        //: euroserv.fr.quakenet.org 433 i1 n1 :Nickname is already in use.
        reply.append(client->getUserInfo().getNickname() + " " +
                     client->getUserInfo().getUsername());
    }
    reply.append(" :Nickname is already in use\r\n");
    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}
