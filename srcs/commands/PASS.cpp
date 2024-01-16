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

void PASS::execute(const std::vector<std::string>& parameters) {
    if (parameters.size() != 1)
        throw std::runtime_error("PASS <password>");
    if (parameters[0] != Message::getPassword())
        throw std::runtime_error("incorrect password.");
    _password = parameters[0];
}

const std::string& PASS::getPassword() const {
    return _password;
}
