#pragma once

#ifndef __KICK_HPP__
#define __KICK_HPP__

#include "Command.hpp"

class KICK : public Command {
  public:
    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);
};

#endif