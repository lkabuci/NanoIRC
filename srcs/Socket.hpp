#pragma once

#include "../include/IRC.h"
#include "AddressResolver.hpp"
#include "ServerHelper.hpp"

#define BACKLOG 10

class Socket {

  public:
    Socket();
    explicit Socket(const char *port);
    int         getSocketFd() const;
    const char *getIP() const;
    const char *getPort() const;

  private:
    int                   _sockfd;
    const char           *_ip;
    const char           *_port;
    const AddressResolver _addresses;

    Socket(const Socket &);
    Socket &operator=(const Socket &);
    void    initializeSocket();
    void    configureSocket();
    void    bindSocket(struct addrinfo) const;
};
