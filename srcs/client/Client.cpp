//
// Created by Redouane Elkaboussi on 1/14/24.
//

#include "Client.hpp"
#include "../server/ServerHelper.hpp"

Client::Client(sockaddr_storage& sockaddr, int sockfd)
    : _sockAddr(sockaddr), _sockfd(sockfd), _ip(), _port(),
      _isDoneReading(false) {
    fillClientIpPort();
    std::cout << "+ add client: (" << sockfd << ") \"" << _ip << "\""
              << std::endl;
}

int Client::getSockfd() const {
    return _sockfd;
}

Client::~Client() {
    std::cout << "- removing client: (" << _sockfd << ") \"" << _ip << "\""
              << std::endl;
    close(_sockfd);
}

void Client::fillClientIpPort() {
    struct sockaddr_in*  ipv4 = NULL;
    struct sockaddr_in6* ipv6 = NULL;

    std::memset(_ip, 0, sizeof _ip);
    std::memset(_port, 0, sizeof _port);

    switch (_sockAddr.ss_family) {
    case AF_INET:
        ipv4 = reinterpret_cast<struct sockaddr_in*>(&_sockAddr);
        inet_ntop(AF_INET, &(ipv4->sin_addr), _ip, sizeof _ip);
        std::snprintf(_port, sizeof(_port), "%d", ntohs(ipv4->sin_port));
        break;
    case AF_INET6:
        ipv6 = reinterpret_cast<struct sockaddr_in6*>(&_sockAddr);
        inet_ntop(AF_INET6, &(ipv6->sin6_addr), _ip, sizeof _ip);
        std::snprintf(_port, sizeof(_port), "%d", ntohs(ipv6->sin6_port));
        break;
    default:
        // TODO: here I should probably return a -1 or throw an error so the
        // poll multiplexing ignore it
        std::cerr << "Unknown address family\n";
        break;
    }
}

const UserInfo& Client::getUserInfo() const {
    return _userInfo;
}

void Client::setUserInfo(const UserInfo& userInfo) {
    _userInfo = userInfo;
}

const std::string& Client::getMessage() const {
    return _message;
}

void Client::appendMessage(const std::string& message) {
    _message.append(message);
}

bool Client::isDoneReading() const {
    return _isDoneReading;
}

void Client::setIsDoneReading(bool isDoneReading) {
    _isDoneReading = isDoneReading;
}
