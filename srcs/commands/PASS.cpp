#include "PASS.hpp"
#include "../parser/Message.hpp"
#include "../server/Server.hpp"

PASS::PASS() {}

PASS::PASS(const PASS& p) : _password(p._password) {}

PASS::~PASS() {}

PASS& PASS::operator=(const PASS& p) {
    if (this == &p)
        return *this;
    _password = p._password;
    return *this;
}

void PASS::execute(Client* client, const std::vector<std::string>& parameters) {
    if (client->getUserInfo().isRegistered()) {
        _errAlreadyRegistred(client);
        return;
    }
    if (!_validParameters(client, parameters))
        return;
    if (parameters[0] != Server::getPasswd()) {
        _errPasswdMismatch(client);
        return;
    }
    _password = parameters[0];
    client->getUserInfo().setPassword(_password);
}

bool PASS::_validParameters(Client*                         client,
                            const std::vector<std::string>& parameters) {
    if (parameters.size() != 1) {
        _errNeedMoreParams(client);
        return false;
    }
    return true;
}

//: tngnet.nl.quakenet.org 461 i1 JOIN :Not enough parameters
void PASS::_errNeedMoreParams(Client* client) {
    std::string reply = ":localhost 461 " +
                        client->getUserInfo().getNickname() +
                        " PASS :Not enough parameters\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void PASS::_errPasswdMismatch(Client* client) {
    std::string reply = ":localhost 464 " +
                        client->getUserInfo().getNickname() +
                        " :Password incorrect\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void PASS::_errAlreadyRegistred(Client* client) {
    // :tngnet.nl.quakenet.org 462 i1 :You may not reregister
    std::string reply = ":localhost 462 " +
                        client->getUserInfo().getNickname() +
                        " :You may not reregister\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}
