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
    if (!Parser::match(TYPES::COLON)) {
        _oneParam();
    } else {
        Parser::skipSpaces();
        while (!Parser::isAtEnd())
            _message.append(Parser::advance().lexeme());
    }
}

void QUIT::_oneParam() {
    _message = Parser::advance().lexeme();

    Parser::skipSpaces();
    if (!Parser::isAtEnd()) {
        Reply::error(_client->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     _client->getUserInfo().getNickname(), "USER");
        throw std::exception();
    }
}

void QUIT::_sendReply() {
    std::string msg = std::string(":") + _client->getUserInfo().getNickname() +
                      "!" + _client->getUserInfo().getUsername() + "@" +
                      Reactor::getInstance().getServerIp() +
                      " QUIT :" + _message + CR_LF;

    send(_client->getSockfd(), msg.c_str(), msg.length(), 0);
}
