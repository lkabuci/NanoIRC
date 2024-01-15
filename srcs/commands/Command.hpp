#pragma once

#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include "../../include/IRC.h"
#include "../parser/ErrorReplies.hpp"
#include "../parser/Parser.hpp"
#include "../parser/Token.hpp"

class Command {
  public:
    virtual ~Command() {}

    virtual void               execute(const std::string& first,
                                       const std::string& parameters) = 0;
    virtual const std::string& get() const = 0;
};

#endif