#include "SendMsg.hpp"

std::string              SendMsg::_textToSend;
std::vector<std::string> SendMsg::_users;
std::vector<std::string> SendMsg::_channels;
Client*                  SendMsg::_sender = NULL;

void SendMsg::sendMessage(Client*                         client,
                          const std::vector<std::string>& parameters) {
    if (parameters.empty()) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_NORECIPIENT,
                     client->getUserInfo().getNickname(), "");
        return;
    }
    if (!client->getUserInfo().isRegistered()) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_NOTREGISTERED,
                     client->getUserInfo().getNickname(), "");
        return;
    }
    _sender = client;
    Parser::init(Utils::join(parameters));
    _parseReceivers();
    _parseText();
    _sendText();
    _clear();
}

void SendMsg::_parseReceivers() {
    while (!Parser::isAtEnd()) {
        if (Parser::check(TYPES::HASH))
            _addChannel();
        else
            _addUser();
        if (Parser::match(TYPES::SPACE))
            break;
        Parser::match(TYPES::COMMA);
    }
}

void SendMsg::_parseText() {
    if (Parser::isAtEnd()) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_NOTEXTTOSEND,
                     _sender->getUserInfo().getNickname(), "");
        throw std::exception();
    }
    if (Parser::match(TYPES::COLON)) {
        Parser::advance(); // skip space
        while (!Parser::isAtEnd())
            _textToSend.append(Parser::advance().lexeme());
    } else {
        _textToSend.append(Parser::advance().lexeme());
        if (!Parser::isAtEnd()) {
            Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                         _sender->getUserInfo().getNickname(),
                         _sender->getUserInfo().getUsername());
            throw std::exception();
        }
    }
    _textToSend.append(CR_LF);
}

void SendMsg::_sendText() {
    for (size_t i = 0; i < _users.size(); ++i) {
        if (_userBelongToChannel(_users[i]))
            continue;
        _sendToUser(_users[i]);
    }
    for (size_t i = 0; i < _channels.size(); ++i) {
        _sendToChannel(_channels[i]);
    }
}

void SendMsg::_sendToUser(const std::string& name) {
    Client* receiver = ClientList::get(name);

    send(receiver->getSockfd(), _textToSend.c_str(), _textToSend.length(), 0);
}

void SendMsg::_sendToChannel(const std::string& name) {
    TChannels::channel(name).sendToAll(_sender, _textToSend);
}

void SendMsg::_addChannel() {
    std::string channel;

    if (!Parser::channel(Parser::peek().lexeme(), channel) ||
        !TChannels::exist(channel)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_NOSUCHCHANNEL,
                     _sender->getUserInfo().getNickname(),
                     Parser::peek().lexeme());
        throw std::exception();
    }
    if (_channelAlreadyExists(channel)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_TOOMANYTARGETS,
                     _sender->getUserInfo().getNickname(), "(PRIVMSG/NOTICE)");
        throw std::exception();
    }
    _channels.push_back(channel);
    Parser::advance();
}

void SendMsg::_addUser() {
    std::string nick;

    if (!Parser::nick(Parser::peek().lexeme(), nick) ||
        !ClientList::exist(nick)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_NOSUCHNICK,
                     _sender->getUserInfo().getNickname(),
                     Parser::peek().lexeme());
        throw std::exception();
    }
    if (_userAlreadyExists(nick)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_TOOMANYTARGETS,
                     _sender->getUserInfo().getNickname(), "(PRIVMSG/NOTICE)");
        throw std::exception();
    }
    if (Parser::peek().lexeme() != _sender->getUserInfo().getNickname())
        _users.push_back(nick);
    Parser::advance();
}

bool SendMsg::_channelAlreadyExists(const std::string& name) {
    return std::find(_channels.begin(), _channels.end(), name) !=
           _channels.end();
}

bool SendMsg::_userAlreadyExists(const std::string& name) {
    return std::find(_users.begin(), _users.end(), name) != _users.end();
}

bool SendMsg::_userBelongToChannel(const std::string& name) {
    for (size_t i = 0; i < _channels.size(); ++i) {
        if (TChannels::channel(_channels[i]).exist(name))
            return true;
    }
    return false;
}

void SendMsg::_clear() {
    _textToSend.clear();
    _users.clear();
    _channels.clear();
    _sender = NULL;
}
