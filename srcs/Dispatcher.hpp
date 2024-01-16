//
// Created by Redouane Elkaboussi on 1/15/24.
//

#ifndef IRC_DISPATCHER_HPP
#define IRC_DISPATCHER_HPP

#include "../include/IRC.h"
#include "Client.hpp"
#include "Reactor.hpp"

class Dispatcher {
  public:
    static void dispatchEvents(std::vector<pollfd> &, std::vector<Client *> &);
};

#endif // IRC_DISPATCHER_HPP
