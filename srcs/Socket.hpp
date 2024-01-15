#pragma once

#include "ServerHelper.hpp"
#include "AddressResolver.hpp"
#include <netdb.h>
#define BACKLOG 10

class Socket {

  public:
    Socket();
    Socket(const char* port);
    int         getSocketFd() const;
    const char* getIP() const;
    const char* getPort() const;

  private:
    int                   _sockfd;
    const char*           _ip;
    const char*           _port;
    const AddressResolver _addresses;

    Socket(const Socket&);
    Socket& operator=(const Socket&);
    void    initializeSocket();
    void    configureSocket();
    void    bindSocket(struct addrinfo) const;
};
