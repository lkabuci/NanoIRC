#include "SendMsg.hpp"

std::string              SendMsg::_textToSend;
std::vector<std::string> SendMsg::_users;
std::vector<std::string> SendMsg::_channels;
Client*                  SendMsg::_sender = NULL;

void SendMsg::sendMessage(Client*                         client,
                          const std::vector<std::string>& parameters) {
    if (parameters.empty()) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_NORECIPIENT, "");
        return;
    }
    if (!client->getUserInfo().isRegistered()) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_NOTREGISTERED, "");
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
        if (Parser::match(TYPES::HASH))
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
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_NOTEXTTOSEND, "");
        return;
    }
    Parser::consume(TYPES::SEMICOLON, "missing semicolon.");
    Parser::advance();
    while (!Parser::isAtEnd())
        _textToSend.append(Parser::advance().lexeme());
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
    if (!Parser::check(TYPES::LETTER))
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_NORECIPIENT, "");
    if (!TChannels::exist(Parser::peek().lexeme()))
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_NOSUCHNICK,
                     Parser::peek().lexeme());
    if (_channelAlreadyExists(Parser::peek().lexeme()))
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_TOOMANYTARGETS,
                     Parser::peek().lexeme());
    if (!Parser::check(TYPES::LETTER)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_NORECIPIENT, "");
        return;
    }
    if (!TChannels::exist(Parser::peek().lexeme())) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_NOSUCHNICK,
                     Parser::peek().lexeme());
        return;
    }
    if (_channelAlreadyExists(Parser::peek().lexeme())) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_TOOMANYTARGETS,
                     Parser::peek().lexeme());
        return;
    }
    _channels.push_back(Parser::advance().lexeme());
}

void SendMsg::_addUser() {
    if (!Parser::check(TYPES::LETTER))
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_TOOMANYTARGETS, "");
    if (!ClientList::exist(Parser::peek().lexeme()))
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_NOSUCHNICK,
                     Parser::peek().lexeme());
    if (_userAlreadyExists(Parser::peek().lexeme()))
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_TOOMANYTARGETS,
                     Parser::peek().lexeme());
    if (!Parser::check(TYPES::LETTER)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_TOOMANYTARGETS, "");
        return;
    }
    if (!ClientList::exist(Parser::peek().lexeme())) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_NOSUCHNICK,
                     Parser::peek().lexeme());
        return;
    }
    if (_userAlreadyExists(Parser::peek().lexeme())) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_TOOMANYTARGETS,
                     Parser::peek().lexeme());
        return;
    }
    if (Parser::peek().lexeme() != _sender->getUserInfo().getNickname())
        _users.push_back(Parser::peek().lexeme());
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
