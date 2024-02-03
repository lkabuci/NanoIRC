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
        //_errAlreadyRegistred(client);
        Reply::errAlreadyRegistred(client);
        return;
    }
    if (!_validParameters(client, parameters))
        return;
    if (parameters[0] != Server::getPasswd()) {
        //_errPasswdMismatch(client);
        Reply::errPasswdMismatch(client);
        return;
    }
    _password = parameters[0];
    client->getUserInfo().setPassword(_password);
}

bool PASS::_validParameters(Client*                         client,
                            const std::vector<std::string>& parameters) {
    if (parameters.size() != 1) {
        //_errNeedMoreParams(client);
        Reply::errNotEnoughParams(client, "PASS");
        return false;
    }
    return true;
}
