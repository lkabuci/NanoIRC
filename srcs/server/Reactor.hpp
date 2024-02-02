//
// Created by Redouane Elkaboussi on 1/15/24.
//

#ifndef IRC_REACTOR_HPP
#define IRC_REACTOR_HPP

#include "../../include/IRC.h"
#include "../client/Client.hpp"
#include "Demultiplexer.hpp"
#include "Dispatcher.hpp"
#include "ServerHelper.hpp"

class Reactor {
  public:
    static Reactor& getInstance();
    ~Reactor();
    void                 addClient(Client*, int socket);
    void                 removeClient(Client*);
    void                 addPfds(pollfd);
    void                 run(const char* port);
    std::vector<pollfd>& getPollfds();
    void                 setServerIp(char* ip);
    const char*          getServerIp();
    static const char*   getServerName();
    std::string          bot(Client*);

  private:
    std::vector<Client*> _clients;
    std::vector<pollfd>  _pollfds;
    char*                _serverIp;
};

#endif // IRC_REACTOR_HPP
