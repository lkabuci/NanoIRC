#include "QUIT.hpp"

QUIT::QUIT() {}

QUIT::~QUIT() {}

void QUIT::execute(Client* client, const std::vector<std::string>& parameters) {
    Parser::init(Utils::join(parameters));
    _client = client;
    _setQuitMessage();
    _sendReply();
    ClientList::remove(client->getUserInfo().getNickname());
}

void QUIT::_setQuitMessage() {
    if (Parser::isAtEnd())
        _message = _client->getUserInfo().getNickname();
    if (!Parser::match(TYPES::COLON)) {
        //_oneParam();
        _message.append(Parser::end().lexeme());
    } else {
        Parser::advance(); // skip space
        while (!Parser::isAtEnd())
            _message.append(Parser::advance().lexeme());
    }
}

void QUIT::_sendReply() {
    std::string msg = std::string(":") + _client->getUserInfo().getNickname() +
                      "!" + _client->getUserInfo().getUsername() + "@" +
                      _client->getIp() + " QUIT :" + _message + CR_LF;

    send(_client->getSockfd(), msg.c_str(), msg.length(), 0);
    try {
        Channel& channel =
            TChannels::userChannel(_client->getUserInfo().getNickname());

        channel.sendToAll(_client, msg);
    } catch (const std::exception& e) {
    }
}
