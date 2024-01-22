#include "USER.hpp"

USER::USER() {}

USER::~USER() {}

void USER::execute(Client* client, const std::vector<std::string>& parameters) {
    if (_notEnoughParams(client, parameters))
        return;

    Parser::init(Utils::join(parameters));

    if (!Parser::name(Parser::advance().lexeme(), _username)) {
        _errErroneousNickname(client, Parser::previous().lexeme());
        return;
    }
    if (!_userSetPassword(client))
        return;
    _ignoreHostAndServerNames();
    _parseRealName(client);
    _setUserInfo(client);
    if (!ClientList::exist(client->getUserInfo().getNickname()))
        ClientList::add(client);
    if (client->getUserInfo().isRegistered())
        _welcome(client);
}

void USER::_errErroneousNickname(Client* client, const std::string& name) {
    //: euroserv.fr.quakenet.org 432 * 2 :Erroneous Nickname
    std::string reply = ":localhost 432 ";

    if (client->getUserInfo().isSet(UserInfo::NICK_SET)) {
        reply.append(client->getUserInfo().getNickname() + " " +
                     client->getUserInfo().getUsername());
    } else {
        reply.append("*");
    }
    reply.append(" " + name + " :Erroneouse Nickname\r\n");
    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

bool USER::_userSetPassword(Client* client) {
    if (client->getUserInfo().isSet(UserInfo::PASSWORD_SET))
        return true;
    std::string reply = ":localhost 451 ";

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

bool USER::_notEnoughParams(Client*                         client,
                            const std::vector<std::string>& parameters) {
    if (parameters.size() < 4) {
        _errNotEnoughParams(client);
        return true;
    }
    return false;
}

void USER::_errNotEnoughParams(Client* client) {
    // USER i2 -> :euroserv.fr.quakenet.org 461 * USER :Not enough parameters
    // nickname set : euroserv.fr.quakenet.org 461 i2 USER :Not enough
    // parameters
    std::string reply = ":localhost 461 ";

    if (client->getUserInfo().getNickname().empty()) {
        reply.append("*");
    } else {
        reply.append(client->getUserInfo().getUsername());
    }
    reply.append(" USER :Not enough parameters\r\n");
    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void USER::_ignoreHostAndServerNames() {
    Parser::advance(); // skip space
    Parser::advance(); // skip hostname
    Parser::advance(); // skip space
    Parser::advance(); // skip servername
}

void USER::_parseRealName(Client* client) {
    if (!Parser::match(TYPES::COLON)) {
        _realname = Parser::advance().lexeme();
    } else {
        while (!Parser::isAtEnd())
            _realname.append(Parser::advance().lexeme());
    }
}

void USER::_setUserInfo(Client* client) {
    client->getUserInfo().setUsername(_username);
    client->getUserInfo().setRealname(_realname);
}

void USER::_welcome(Client* client) {
    std::string msg = std::string(":") + "localhost 001 " +
                      client->getUserInfo().getNickname() +
                      " :Welcome to the IRCServer network, " +
                      client->getUserInfo().getNickname() + "!" +
                      client->getUserInfo().getUsername() + "@localhost\r\n";

    send(client->getSockfd(), msg.c_str(), msg.length(), 0);
}
