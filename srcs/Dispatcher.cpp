//
// Created by Redouane Elkaboussi on 1/15/24.
//

#include "Dispatcher.hpp"
#include "ClientHandler.hpp"

#define SERVER 0

/*
 * RFC 1459 #section-2.3
 *
 * 510 is the Max _message length
 * 512 is for the last trailing CRLF(\r\n)
 * 513 a byte extra for protection if exceeded the limits
 */
void Dispatcher::dispatchEvents(std::vector<pollfd>   &fds,
                                std::vector<Client *> &clients) {
    for (size_t i = 0; i < fds.size(); ++i) {
        // TODO: add POLLHUP POLLERROR
        if (fds[i].revents & POLLIN) {
            if (i == SERVER) {
                ClientHandler::handleNewConnection(fds[i]);
                continue;
            }
            ClientHandler::handleClientInput(clients[i - 1]);
        }
    }
}
