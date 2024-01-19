#include "NOTICE.hpp"
#include "SendMsg.hpp"

NOTICE::NOTICE() {}

NOTICE::~NOTICE() {}

void NOTICE::execute(Client*                         client,
                     const std::vector<std::string>& parameters) {
    SendMsg::sendMessage(client, parameters);
}
