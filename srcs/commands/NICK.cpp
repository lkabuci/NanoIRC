#include "NICK.hpp"

NICK::NICK() {}

NICK::~NICK() {}

void NICK::execute(Client* client, const std::vector<std::string>& parameters) {
    if (_notEnoughParams(client, parameters) || !_userSetPassword(client))
        return;
    if (!Parser::name(parameters[0], _nick)) {
        _errErroneousNickname(client, parameters[0]);
        return;
    }
    if (_nickIsSame(client) || _nicknameAlreadyInUse(client))
        return;
    if (_nicknameChange(client)) {
        _change(client);
    } else {
        _newUser(client);
    }
}

void NICK::_newUser(Client* client) {
    client->getUserInfo().setNickname(_nick);
    ClientList::add(client);
    if (client->getUserInfo().isRegistered())
        _welcome(client);
}

bool NICK::_nicknameChange(Client* client) {
    return !client->getUserInfo().getNickname().empty();
}

void NICK::_change(Client* client) {
    _rpl(client);
    ClientList::changeNickname(client->getUserInfo().getNickname(), _nick);
    client->getUserInfo().setNickname(_nick);
}

bool NICK::_nickIsSame(Client* client) {
    return client->getUserInfo().getNickname() == _nick;
}

bool NICK::_nicknameAlreadyInUse(Client* client) {
    if (ClientList::exist(_nick)) {
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
    std::string reply = ":ircserver 451 ";

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
    std::time_t tm = std::time(0);
    char*       date_time = std::ctime(&tm);
    std::string msg = ":ircserver 001 " + _nick + " :sf ghayerha\r\n";
    std::string yourhost = ":ircserver 002 " + _nick +
                           " :Your host is ircserver, running version i1\r\n";
    std::string created = ":ircserver 003 " + _nick +
                          " :This server was created " + date_time + CR_LF;
    std::string info = ":ircserver 004 " + _nick + " ircserver i1 itkol\r\n";

    send(client->getSockfd(), msg.c_str(), msg.length(), 0);
    send(client->getSockfd(), yourhost.c_str(), yourhost.length(), 0);
    send(client->getSockfd(), created.c_str(), created.length(), 0);
    send(client->getSockfd(), info.c_str(), info.length(), 0);
}

//: i1!~u1@197.230.30.146 NICK :fhfhfhfh
void NICK::_rpl(Client* client) {
    std::string reply = std::string(":") + client->getUserInfo().getNickname() +
                        "!~" + client->getUserInfo().getUsername() + "@" +
                        Reactor::getInstance().getServerIp() +
                        " NICK :" + _nick + CR_LF;

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void NICK::_errNoNicknameGiven(Client* client) {
    std::string nick = _nick.empty() ? "*" : _nick;
    std::string reply = ":ircserver 431 " + nick + " :No nickname given\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void NICK::_errNicknameAlreadyInUse(Client* client) {
    std::string reply = ":ircserver 433 ";

    if (client->getUserInfo().getNickname().empty()) {
        reply.append("*");
    } else {
        //: euroserv.fr.quakenet.org 433 i1 n1 :Nickname is already in use.
        reply.append(client->getUserInfo().getNickname() + " " +
                     client->getUserInfo().getUsername());
    }
    reply.append(" " + _nick + " :Nickname is already in use\r\n");
    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void NICK::_errErroneousNickname(Client* client, const std::string& name) {
    //: stockholm.se.quakenet.org 432 * 23g :Erroneous Nickname
    std::string reply = ":ircserver 432 ";

    if (client->getUserInfo().getNickname().empty()) {
        reply.append("* ");
    } else {
        //: euroserv.fr.quakenet.org 433 i1 n1 :Nickname is already in use.
        reply.append(client->getUserInfo().getNickname());
    }
    reply.append(name == ":" ? "" : name);
    reply.append(" :Erroneous Nickname\r\n");
    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}
