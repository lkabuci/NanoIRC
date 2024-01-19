#pragma once

#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include "../Reply/Reply.hpp"
#include "../Utils.hpp"
#include "../channels/TChannels.hpp"
#include "../client/ClientList.hpp"
#include "../parser/Parser.hpp"
#include "../server/Reactor.hpp"

class Command {
  public:
    virtual ~Command() {}

    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters) = 0;
};

#endif
