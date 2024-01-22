#include "SendMsg.hpp"

std::string              SendMsg::_cmd;
std::string              SendMsg::_textToSend;
std::vector<std::string> SendMsg::_users;
std::vector<std::string> SendMsg::_channels;
Client*                  SendMsg::_sender = NULL;

void SendMsg::sendMessage(Client*                         client,
                          const std::vector<std::string>& parameters,
                          const std::string&              command) {
    _cmd = command;
    if (parameters.empty()) {
        //: stockholm.se.quakenet.org 411 i2 :No recipient given (PRIVMSG)
        _errNoRecipent(client);
        return;
    }
    if (!client->getUserInfo().isRegistered()) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_NOTREGISTERED,
                     client->getUserInfo().getNickname(), "");
        return;
    }
    _sender = client;
    Parser::init(Utils::join(parameters));
    try {
        _parseReceivers();
        _parseText();
        _sendText();
    } catch (const std::exception& e) {
    }
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
    if (Parser::isAtEnd())
        _errNoTextToSend();
    if (Parser::match(TYPES::COLON)) {
        Parser::advance(); // skip space
        while (!Parser::isAtEnd())
            _textToSend.append(Parser::advance().lexeme());
    } else {
        //: i2!~u2@197.230.30.146 PRIVMSG i1 :you
        _textToSend.append(Parser::end().lexeme());
    }
    _textToSend.append(CR_LF);
}

void SendMsg::_sendText() {
    for (size_t i = 0; i < _users.size(); ++i) {
        _sendToUser(_users[i]);
    }
    for (size_t i = 0; i < _channels.size(); ++i) {
        _sendToChannel(_channels[i]);
    }
}

void SendMsg::_sendToUser(const std::string& name) {
    Client*     receiver = ClientList::get(name);
    std::string msg = ":" + _sender->getUserInfo().getNickname() + "!~" +
                      _sender->getUserInfo().getUsername() + "@localhost " +
                      _cmd + " " + name + " :" + _textToSend;

    //: i2!~u2@197.230.30.146 PRIVMSG i1 :hi
    send(receiver->getSockfd(), msg.c_str(), msg.length(), 0);
}

void SendMsg::_sendToChannel(const std::string& name) {
    Channel& channel = TChannels::channel(name);

    channel.sendToAll(_sender, _textToSend);
    _sendChannelReply(channel);
}

//: i1!~u1@197.230.30.146 PRIVMSG #ch1 :  hello
void SendMsg::_sendChannelReply(Channel& channel) {
    std::string msg = ":" + _sender->getUserInfo().getNickname() + "!~" +
                      _sender->getUserInfo().getUsername() + "@" +
                      Reactor::getInstance().getServerIp() + " " + _cmd + " " +
                      channel.name() + " " + _textToSend + CR_LF;

    channel.sendToAll(_sender, msg);
}

void SendMsg::_addChannel() {
    std::string channel = Parser::peek().lexeme();

    if (!TChannels::exist(channel)) {
        _errNoSuch(channel, "No such channel");
    }
    _channels.push_back(channel);
    Parser::advance();
}

void SendMsg::_addUser() {
    std::string nick = Parser::peek().lexeme();

    if (!ClientList::exist(nick)) {
        //: stockholm.se.quakenet.org 401 i2 i414 :No such nick
        _errNoSuch(nick, "No such nick");
    }
    if (Parser::peek().lexeme() != _sender->getUserInfo().getNickname())
        _users.push_back(nick);
    Parser::advance();
}

void SendMsg::_clear() {
    _textToSend.clear();
    _users.clear();
    _channels.clear();
    _sender = NULL;
}

void SendMsg::_errNoRecipent(Client* client) {
    std::string reply = ":localhost 411 " +
                        client->getUserInfo().getNickname() +
                        " :No recipent given (" + _cmd + ")\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

//: stockholm.se.quakenet.org 412 i2 :No text to send
void SendMsg::_errNoTextToSend() {
    std::string msg = std::string(":localhost") + " 412 " +
                      _sender->getUserInfo().getNickname() +
                      " :No text to send\r\n";

    send(_sender->getSockfd(), msg.c_str(), msg.length(), 0);
    throw std::exception();
}

void SendMsg::_errNoSuch(const std::string& name,
                         const std::string& description) {
    std::string reply = ":localhost 403 " +
                        _sender->getUserInfo().getNickname() + " " + name +
                        " :" + description + CR_LF;

    send(_sender->getSockfd(), reply.c_str(), reply.length(), 0);
    throw std::exception();
}
