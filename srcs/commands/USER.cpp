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
    if (parameters.size() < 2)
        throw std::runtime_error("USER <username> <realname>");
    if (!client->getUserInfo().isSet(UserInfo::PASSWORD_SET))
        throw std::runtime_error("client is not registered.");
    Parser::init(Utils::join(parameters));
    _username = Parser::advance().lexeme();
    Parser::consume(TYPES::SPACE, "missing space.");
    _parseRealName();
    client->getUserInfo().setUsername(_username);
    client->getUserInfo().setRealname(_realname);
}

void USER::_parseRealName() {
    Parser::consume(TYPES::SEMICOLON, "missing semicolon.");
    while (!Parser::isAtEnd())
        _realname.append(Parser::advance().lexeme());
}

const std::string& USER::getUsername() const {
    return _username;
}

const std::string& USER::getRealname() const {
    return _realname;
}
