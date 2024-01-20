#include "PRIVMSG.hpp"
#include "SendMsg.hpp"

PRIVMSG::PRIVMSG() {}

PRIVMSG::~PRIVMSG() {}

void PRIVMSG::execute(Client*                         client,
                      const std::vector<std::string>& parameters) {
    SendMsg::sendMessage(client, parameters);
}
