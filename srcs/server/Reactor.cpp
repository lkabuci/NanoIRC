//
// Created by Redouane Elkaboussi on 1/15/24.
//

#include "Reactor.hpp"
#include "../bot/Bot.hpp"

Reactor& Reactor::getInstance() {
    static Reactor instance;
    return instance;
}

Reactor::~Reactor() {
    for (std::vector<Client*>::iterator it = _clients.begin();
         it != _clients.end(); ++it) {
        delete *it;
    }
    _clients.clear();
    _pollfds.clear();
}

void Reactor::addClient(Client* client, int socket) {
    _clients.push_back(client);
    pollfd newPfd = {socket, POLLIN, 0};
    _pollfds.push_back(newPfd);
}

void Reactor::removeClient(Client* client) {
    std::vector<Client*>::iterator it =
        std::find(_clients.begin(), _clients.end(), client);
    if (it != _clients.end()) {
        long index = std::distance(_clients.begin(), it);

        delete *it;
        *it = NULL;
        _clients.erase(_clients.begin() + index);
        _pollfds.erase(_pollfds.begin() + index + 1);
    }
}

void Reactor::run(const char* port) {
    BOT bot(port);
    bot.addToClients();
    while (serverIsRunning) {
        if (Demultiplexer::waitForEvents() == -1) {
            break;
        }
        Dispatcher::dispatchEvents(_pollfds, _clients);
    }
}

std::vector<pollfd>& Reactor::getPollfds() {
    return _pollfds;
}

void Reactor::addPfds(pollfd pfd) {
    _pollfds.push_back(pfd);
}

void Reactor::setServerIp(char* ip) {
    _serverIp = ip;
}

const char* Reactor::getServerIp() {
    return _serverIp;
}

std::string Reactor::bot(Client* client) {
    // append CRLF
    send(client->getSockfd(), "hhh", 3, 0);
    return std::string("hhh");
}

const char* Reactor::getServerName() {
    return "e1r7p15.ma";
}
