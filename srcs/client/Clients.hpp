#pragma once

#ifndef __CLIENTS_HPP__
#define __CLIENTS_HPP__

#include "Client.hpp"

class Clients {
  public:
    static void    add(Client* client);
    static Client* get(const std::string& nickname);
    static Client* get(const size_t& index);
    static void    remove(const std::string& nickname);
    static size_t  numberOfClients();
    static bool    exist(const std::string& nickname);
    static size_t  size();

  private:
    Clients();

    static std::map<std::string, Client*> _clients;
};

#endif
