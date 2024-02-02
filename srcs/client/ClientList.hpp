#pragma once

#ifndef __CLIENT_LIST_HPP__
#define __CLIENT_LIST_HPP__

#include "Client.hpp"

class ClientList {
  public:
    static void    add(Client* client);
    static Client* get(const std::string& nickname);
    static Client* get(const size_t& index);
    static void    remove(const std::string& nickname);
    static size_t  numberOfClients();
    static bool    exist(const std::string& nickname);
    static size_t  size();
    static void    changeNickname(const std::string& oldName,
                                  const std::string& newName);

  private:
    ClientList();

    static std::map<std::string, Client*> _clients;
};

#endif
