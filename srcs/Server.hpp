//
// Created by Redouane Elkaboussi on 1/14/24.
//

#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include "../include/IRC.h"
#include "Client.hpp"
#include "Socket.hpp"

class Server {
  public:
    explicit Server(const char *port);
    void run();

  private:
    Socket _sock;
};

#endif // IRC_SERVER_HPP
