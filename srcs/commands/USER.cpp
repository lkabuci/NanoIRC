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
    if (_notEnoughParams(client, parameters))
        return;

    Parser::init(Utils::join(parameters));

    _username = Parser::advance().lexeme();
    if (_userAlreadyExists(client) || !_userSetPassword(client))
        return;
    if (!Parser::match(TYPES::SPACE)) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     client->getUserInfo().getNickname(), "USER");
        return;
    }
    _ignoreHostAndServerNames();
    _parseRealName(client);
    _setUserInfo(client);
    if (!ClientList::exist(client->getUserInfo().getNickname()))
        ClientList::add(client);
    if (client->getUserInfo().isRegistered())
        _welcomeUser(client);
}

bool USER::_userAlreadyExists(Client* client) {
    if (ClientList::exist(_username)) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_ALREADYREGISTRED,
                     client->getUserInfo().getNickname(), "");
        return true;
    }
    return false;
}

bool USER::_userSetPassword(Client* client) {
    if (client->getUserInfo().isSet(UserInfo::PASSWORD_SET))
        return true;
    Reply::error(client->getSockfd(), ERROR_CODES::ERR_NOTREGISTERED,
                 client->getUserInfo().getNickname(), "");
    return false;
}

bool USER::_notEnoughParams(Client*                         client,
                            const std::vector<std::string>& parameters) {
    if (parameters.size() < 4)
        return true;
    Reply::error(client->getSockfd(), ERROR_CODES::ERR_NEEDMOREPARAMS,
                 client->getUserInfo().getNickname(), "USER");
    return false;
}

void USER::_ignoreHostAndServerNames() {
    Parser::advance(); // skip space
    Parser::advance(); // skip hostname
    Parser::advance(); // skip space
    Parser::advance(); // skip servername
}

void USER::_parseRealName(Client* client) {
    if (!Parser::match(TYPES::SEMICOLON)) {
        _oneParam(client);
    } else {
        while (!Parser::isAtEnd())
            _realname.append(Parser::advance().lexeme());
    }
}

void USER::_oneParam(Client* client) {
    _realname = Parser::advance().lexeme();

    if (!Parser::isAtEnd() && !Parser::check(TYPES::SPACE)) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     client->getUserInfo().getNickname(), "USER");
        throw std::exception();
    }
}

void USER::_setUserInfo(Client* client) {
    client->getUserInfo().setUsername(_username);
    client->getUserInfo().setRealname(_realname);
}

void USER::_welcomeUser(Client* client) {
    Reply::rpl_welcome(client->getSockfd(), client->getUserInfo().getNickname(),
                       client->getUserInfo().getUsername());
}
