#include "NOTICE.hpp"

NOTICE::NOTICE() {}

NOTICE::~NOTICE() {}

void NOTICE::execute(Client*                         client,
                     const std::vector<std::string>& parameters) {
    if (parameters.empty())
        return;
    Parser::init(Utils::join(parameters));
    if (Parser::match(TYPES::HASH))
        _sendToChannel();
    else
        _sendToUser(client);
}

void NOTICE::_sendToChannel() {
    std::string channel_name = Parser::advance().lexeme();

    if (!TChannels::exist(channel_name))
        return;
    std::string message;

    Parser::skipSpaces();
    while (!Parser::isAtEnd())
        message.append(Parser::advance().lexeme());
    TChannels::channel(channel_name).sendToAll(message);
}

void NOTICE::_sendToUser(Client* client) {
    std::string user_name = Parser::advance().lexeme();
    Client*     user = ClientList::get(user_name);

    if (!user)
        return;
    if (client->getUserInfo().getNickname() == user_name)
        return;
    std::string message;

    Parser::skipSpaces();
    while (!Parser::isAtEnd())
        message.append(Parser::advance().lexeme());
    send(user->getSockfd(), message.c_str(), message.length(), 0);
}
