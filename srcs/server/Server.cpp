//
// Created by Redouane Elkaboussi on 1/14/24.
//

#include "Server.hpp"
#include "Reactor.hpp"

std::string Server::_passwd;

Server::Server(const char* port, const char* passwd)
    : _sock(port), _port(port) {
    Utils::isAllDigits(port);
    pollfd serverPollfd = {_sock.getSocketFd(), POLLIN, 0};
    Reactor::getInstance().addPfds(serverPollfd);
}

void Server::run() {
    Reactor::getInstance().run(_port);
}

const std::string& Server::getPasswd() {
    return _passwd;
}

Server::~Server() {
    std::cout << "Closing server\n";
    close(_sock.getSocketFd());
}
