#pragma once

#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include "../parser/Parser.hpp"
#include "Channel.hpp"

class Command {
  public:
    virtual ~Command() {}

    virtual void execute(const std::vector<std::string>& parameters) = 0;
};

#endif
