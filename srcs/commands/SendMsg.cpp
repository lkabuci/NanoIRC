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
    _sender = client;
    if (parameters.empty()) {
        //: stockholm.se.quakenet.org 411 i2 :No recipient given (PRIVMSG)
        _errNoRecipent();
        return;
    }
    if (!client->getUserInfo().isRegistered()) {
        //: stockholm.se.quakenet.org 451 *  :Register first
        _errNotRegistered();
        return;
    }
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
    //: i1!~u1@197.230.30.146 PRIVMSG #ch1 :  hello
    std::string msg = ":" + _sender->getUserInfo().getNickname() + "!~" +
                      _sender->getUserInfo().getUsername() + "@" +
                      Reactor::getInstance().getServerIp() + " " + _cmd + " " +
                      name + " :" + _textToSend + CR_LF;

    channel.sendToAll(_sender, msg);
}

void SendMsg::_addChannel() {
    std::string channel = Parser::advance().lexeme();

    if (!Parser::isAtEnd() && !Parser::check(TYPES::COMMA))
        channel.append(Parser::advance().lexeme());
    if (!TChannels::exist(channel)) {
        _errNoSuch(channel, "No such channel");
        return;
    }
    _channels.push_back(channel);
}

void SendMsg::_addUser() {
    std::string nick = Parser::peek().lexeme();

    if (!ClientList::exist(nick)) {
        //: stockholm.se.quakenet.org 401 i2 i414 :No such nick
        _errNoSuch(nick, "No such nick");
        return;
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

void SendMsg::_errNoRecipent() {
    if (_cmd == "NOTICE")
        return;
    std::string reply = ":localhost 411 " +
                        _sender->getUserInfo().getNickname() +
                        " :No recipent given (" + _cmd + ")\r\n";

    send(_sender->getSockfd(), reply.c_str(), reply.length(), 0);
}

//: stockholm.se.quakenet.org 412 i2 :No text to send
void SendMsg::_errNoTextToSend() {
    if (_cmd == "NOTICE")
        throw std::exception();
    std::string msg = std::string(":localhost") + " 412 " +
                      _sender->getUserInfo().getNickname() +
                      " :No text to send\r\n";

    send(_sender->getSockfd(), msg.c_str(), msg.length(), 0);
    throw std::exception();
}

void SendMsg::_errNoSuch(const std::string& name,
                         const std::string& description) {
    if (_cmd == "NOTICE")
        throw std::exception();
    std::string reply = ":localhost 403 " +
                        _sender->getUserInfo().getNickname() + " " + name +
                        " :" + description + CR_LF;

    send(_sender->getSockfd(), reply.c_str(), reply.length(), 0);
    throw std::exception();
}

void SendMsg::_errNotRegistered() {
    if (_cmd == "NOTICE")
        return;
    std::string reply = ":localhost 451 ";

    if (_sender->getUserInfo().getNickname().empty()) {
        reply.append("*");
    } else {
        reply.append(_sender->getUserInfo().getNickname() + " " +
                     _sender->getUserInfo().getUsername());
    }
    reply.append(" :You have not regisetred\r\n");

    send(_sender->getSockfd(), reply.c_str(), reply.length(), 0);
}
