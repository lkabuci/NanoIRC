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
    (void)client;
    std::string message = Utils::join(parameters);

    Parser::init(message);
    _parseReceivers();
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

void PRIVMSG::_addChannel() {
    if (!Parser::check(TYPES::LETTER))
        throw std::runtime_error(
            "411 ERR_NORECIPIENT::No recipient given (<command>)");
    if (!TChannels::exist(Parser::peek().lexeme()))
        throw std::runtime_error(
            "401 ERR_NOSUCHNICK:<nickname> :No such nick/channel");
    if (std::find(_channels.begin(), _channels.end(),
                  Parser::peek().lexeme()) != _channels.end())
        throw std::runtime_error("407 ERR_TOOMANYTARGETS:<target> :Duplicate "
                                 "recipients. No message delivered");
    _channels.push_back(Parser::advance().lexeme());
}

void PRIVMSG::_addUser() {
    if (!Parser::check(TYPES::LETTER))
        throw std::runtime_error(
            "411 ERR_NORECIPIENT::No recipient given (<command>)");
    if (!Clients::exist(Parser::peek().lexeme()))
        throw std::runtime_error(
            "401 ERR_NOSUCHNICK:<nickname> :No such nick/channel");
    if (std::find(_users.begin(), _users.end(), Parser::peek().lexeme()) !=
        _users.end())
        throw std::runtime_error("407 ERR_TOOMANYTARGETS:<target> :Duplicate "
                                 "recipients. No message delivered");
    _users.push_back(Parser::advance().lexeme());
}
