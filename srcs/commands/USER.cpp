#include "USER.hpp"

USER::USER() {}

USER::USER(const USER& user)
    : _username(user._username), _realname(user._realname) {}

USER::~USER() {}

USER& USER::operator=(const USER& user) {
    if (this == &user)
        return *this;
    _username = user._username;
    _realname = user._realname;
    return *this;
}

void USER::execute(Client* client, const std::vector<std::string>& parameters) {
    (void)client;
    if (parameters.size() < 3)
        throw std::runtime_error("USER <username> <realname>");
    _username = parameters[0];
    if (parameters[1] != ":")
        throw std::runtime_error("missing semicolon for realname.");
    for (size_t i = 2; i < parameters.size(); ++i)
        _realname.append(parameters[i]);
}

const std::string& USER::getUsername() const {
    return _username;
}

const std::string& USER::getRealname() const {
    return _realname;
}
