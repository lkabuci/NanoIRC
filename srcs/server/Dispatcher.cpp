//
// Created by Redouane Elkaboussi on 1/15/24.
//

#include "Dispatcher.hpp"
#include "../client/ClientHandler.hpp"
#include "Reactor.hpp"

#define SERVER 0

void Dispatcher::dispatchEvents(std::vector<pollfd>&  fds,
                                std::vector<Client*>& clients) {
    for (size_t i = 0; i < fds.size(); ++i) {
        if (fds[i].revents & POLLHUP) {
            if (i == SERVER) {
                serverIsRunning = 0;
                break;
            }
            Reactor::getInstance().removeClient(clients[i - 1]);
            continue;
        }
        if (fds[i].revents & POLLIN) {
            if (i == SERVER) {
                ClientHandler::handleNewConnection(fds[i]);
                continue;
            }
            ClientHandler::handleClientInput(clients[i - 1]);
        }
    }
}
