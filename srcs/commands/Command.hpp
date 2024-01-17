#pragma once

#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include "../Utils.hpp"
#include "../parser/Parser.hpp"
#include "../client/Client.hpp"

class Command {
  public:
    virtual ~Command() {}

    virtual void execute(Client* client,
                        const std::vector<std::string>& parameters) = 0;
};

#endif
