#include "Bot.hpp"
#include "../server/Reactor.hpp"
#include "../socket/AddressResolver.hpp"

BOT::BOT(const char* port) : _address(port) {
    int             botSockFd;
    const addrinfo* botInfo = _address.getAddresses();
    const addrinfo* p = botInfo;
    for (; p != NULL; p = p->ai_next) {
        if ((botSockFd =
                 socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("bot: socket");
            continue;
        }
        if (::connect(botSockFd, p->ai_addr, p->ai_addrlen) == -1) {
            close(botSockFd);
            perror("bot: connect");
            continue;
        }
        break;
    }
    if (p == NULL) {
        std::cerr << "Bot failed to connect\n";
        return;
    }
}

void BOT::addToClients() {
    int serverfd = Reactor::getInstance().getPollfds()[0].fd;

    sockaddr_storage botAddrs;
    socklen_t        botAddrLen = sizeof(botAddrs);
    int              botSocket =
        accept(serverfd, reinterpret_cast<sockaddr*>(&botAddrLen), &botAddrLen);

    if (botSocket == -1) {
        std::cerr << "Failed to accept new connection" << std::endl;
        return;
    }

    Reactor::getInstance().addClient(new Client(botAddrs, botSocket),
                                     botSocket);
}
