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
        // TODO sent ERR_ALREADYREGISTRED reply and ignore the command
        throw std::runtime_error(
            "462 ERR_ALREADYREGISTRED:You may not reregister");
    }
    if (parameters.size() != 1) {
        // TODO sent ERR_NEEDMOREPARAMS reply
        throw std::runtime_error(
            "462 ERR_NEEDMOREPARAMS:You may not reregister");
    }
    _password = parameters[0];
    client->getUserInfo().setPassword(_password);
}

const std::string& PASS::getPassword() const {
    return _password;
}
