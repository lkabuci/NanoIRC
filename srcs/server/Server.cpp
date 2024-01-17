//
// Created by Redouane Elkaboussi on 1/14/24.
//

#include "Server.hpp"
#include "Reactor.hpp"

Server::Server(const char* port) : _sock(port), _port(port) {
    pollfd serverPollfd = {_sock.getSocketFd(), POLLIN, 0};
    Reactor::getInstance().addPfds(serverPollfd);
}

void Server::run() {
    Reactor::getInstance().run(_port);
}
