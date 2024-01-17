#pragma once

#include <vector>
#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include "../../include/IRC.h"
#include "../client/Client.hpp"

class Channel {
  public:
    void add(const Client& newMember);

  private:
    std::vector<Client> _members;
};

#endif