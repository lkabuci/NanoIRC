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
    if (!_validParameters(client->getSockfd(), parameters))
        return;

    _password = parameters[0];
    client->getUserInfo().setPassword(_password);
}

bool PASS::_validParameters(int fd, const std::vector<std::string>& parameters) {
    if (parameters.empty()) {
        Reply::error(fd, ERROR_CODES::ERR_UNKNOWNCOMMAND, "PASS");
        return false;
    }
    if (parameters.size() != 1) {
        Reply::error(fd, ERROR_CODES::ERR_NEEDMOREPARAMS, "PASS");
        return false;
    }
    return true;
}
