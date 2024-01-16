//
// Created by Redouane Elkaboussi on 1/15/24.
//

#ifndef IRC_DEMULTIPLEXER_HPP
#define IRC_DEMULTIPLEXER_HPP

#include "../include/IRC.h"
#include "Reactor.hpp"

class Demultiplexer {
  public:
    static int waitForEvents();
};

#endif // IRC_DEMULTIPLEXER_HPP
