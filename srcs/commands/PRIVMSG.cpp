#include "PRIVMSG.hpp"

PRIVMSG::PRIVMSG() {}

PRIVMSG::PRIVMSG(const PRIVMSG& privmsg)
    : _users(privmsg._users), _channels(privmsg._channels),
      _textToSent(privmsg._textToSent) {}

PRIVMSG::~PRIVMSG() {}

PRIVMSG& PRIVMSG::operator=(const PRIVMSG& privmsg) {
    if (this == &privmsg)
        return *this;
    _users = privmsg._users;
    _channels = privmsg._channels;
    _textToSent = privmsg._textToSent;
    return *this;
}

void PRIVMSG::execute(Client*                         client,
                      const std::vector<std::string>& parameters) {
    if (parameters.empty())
        return;
    _sender = client;
    std::string message = Utils::join(parameters);

    Parser::init(message);
    _parseReceivers();
    _parseText();
    _sentText();
    return;
}

void PRIVMSG::_parseReceivers() {
    while (!Parser::isAtEnd() && !Parser::check(TYPES::SPACE)) {
        if (Parser::match(TYPES::HASH))
            _addChannel();
        else
            _addUser();
        Parser::consume(TYPES::COMMA, "missing comma.");
    }
}

void PRIVMSG::_parseText() {
    if (Parser::isAtEnd())
        throw std::runtime_error("412 ERR_NOTEXTTOSEND:No text to send");
    Parser::consume(TYPES::SEMICOLON, "missing semicolon.");
    while (!Parser::isAtEnd())
        _textToSent.append(Parser::advance().lexeme());
}

void PRIVMSG::_sentText() {
    for (size_t i = 0; i < _users.size(); ++i) {
        if (_userBelongToChannel(_users[i]))
            continue;
        _sendToUser(_users[i]);
    }
}

void PRIVMSG::_sendToUser(const std::string& name) {
    // Client*     receiver = TChannels::c

    // if (!receiver)
    //     throw std::runtime_error("user not in channel.");
    // send(receiver->getSockfd(), _textToSent.c_str(), _textToSent.length(),
    // 0);
}

bool PRIVMSG::_userBelongToChannel(const std::string& name) {
    for (size_t i = 0; i < _channels.size(); ++i) {
        if (TChannels::channel(_channels[i]).exist(name))
            return true;
    }
    return false;
}

void PRIVMSG::_addChannel() {
    if (!Parser::check(TYPES::LETTER))
        throw std::runtime_error(
            "411 ERR_NORECIPIENT::No recipient given (<command>)");
    if (!TChannels::exist(Parser::peek().lexeme()))
        throw std::runtime_error(
            "401 ERR_NOSUCHNICK:<nickname> :No such nick/channel");
    if (_channelAlreadyExists(Parser::peek().lexeme()))
        throw std::runtime_error("407 ERR_TOOMANYTARGETS:<target> :Duplicate "
                                 "recipients. No message delivered");
    _channels.push_back(Parser::advance().lexeme());
}

bool PRIVMSG::_channelAlreadyExists(const std::string& name) {
    return std::find(_channels.begin(), _channels.end(), name) !=
           _channels.end();
}

void PRIVMSG::_addUser() {
    if (!Parser::check(TYPES::LETTER))
        throw std::runtime_error(
            "411 ERR_NORECIPIENT::No recipient given (<command>)");
    if (!Clients::exist(Parser::peek().lexeme()))
        throw std::runtime_error(
            "401 ERR_NOSUCHNICK:<nickname> :No such nick/channel");
    if (_userAlreadyExists(Parser::peek().lexeme()))
        throw std::runtime_error("407 ERR_TOOMANYTARGETS:<target> :Duplicate "
                                 "recipients. No message delivered");
    if (Parser::peek().lexeme() != _sender->getUserInfo().getNickname())
        _users.push_back(Parser::advance().lexeme());
}

bool PRIVMSG::_userAlreadyExists(const std::string& name) {
    return std::find(_users.begin(), _users.end(), name) != _users.end();
}
