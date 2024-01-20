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
    if (parameters.size() != 4) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_NEEDMOREPARAMS,
                     "USER");
        return;
    }
    if (ClientList::exist(_username)) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_ALREADYREGISTRED,
                     "");
        return;
    }
    if (!client->getUserInfo().isSet(UserInfo::PASSWORD_SET)) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_NOTEXTTOSEND, "");
        return;
    }
    Parser::init(Utils::join(parameters));

    _username = Parser::advance().lexeme();
    Parser::consume(TYPES::SPACE, "missing space.");
    _ignoreHostAndServerNames();
    _parseRealName();
    client->getUserInfo().setUsername(_username);
    client->getUserInfo().setRealname(_realname);
    if (!ClientList::exist(client->getUserInfo().getNickname()))
        ClientList::add(client);
    _sendSuccessReply(client);
}

void USER::_sendSuccessReply(Client* client) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      " 001 " + client->getUserInfo().getNickname() +
                      ":Welcome to IRC " + client->getUserInfo().getNickname() +
                      "!" + _username + "@" +
                      Reactor::getInstance().getServerIp() + "\r\n";

    send(client->getSockfd(), msg.c_str(), msg.length(), 0);
}

void USER::_ignoreHostAndServerNames() {
    Parser::advance(); // skip space
    Parser::advance(); // skip hostname
    Parser::advance(); // skip space
    Parser::advance(); // skip servername
}

void USER::_parseRealName() {
    // Parser::consume(TYPES::SEMICOLON, "missing semicolon.");
    while (!Parser::isAtEnd())
        _realname.append(Parser::advance().lexeme());
}

const std::string& USER::getUsername() const {
    return _username;
}

const std::string& USER::getRealname() const {
    return _realname;
}
