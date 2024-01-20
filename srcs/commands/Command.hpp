#pragma once

#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include "../Utils/Utils.hpp"
#include "../channels/TChannels.hpp"
#include "../client/ClientList.hpp"
#include "../Utils/Utils.hpp"
#include "../parser/Parser.hpp"
#include "../reply/Reply.hpp"
#include "../server/Reactor.hpp"

class Command {
  public:
    virtual ~Command() {}

    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters) = 0;
};

#endif
