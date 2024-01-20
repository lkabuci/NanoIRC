#include "PASS.hpp"
#include "../parser/Message.hpp"

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
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_ALREADYREGISTRED,
                     "");
        return;
    }
    if (!_validParameters(client, parameters))
        return;

    _password = parameters[0];
    client->getUserInfo().setPassword(_password);
}

bool PASS::_validParameters(Client*                         client,
                            const std::vector<std::string>& parameters) {
    if (parameters.empty()) {
        _sendErrorReply(client);
        return false;
    }
    if (parameters.size() != 1) {
        _sendErrorReply(client);
        return false;
    }
    return true;
}

void PASS::_sendErrorReply(Client* client) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      " 461 " + client->getUserInfo().getNickname() +
                      " PASS :Not enough parameters\r\n";

    send(client->getSockfd(), msg.c_str(), msg.length(), 0);
}
