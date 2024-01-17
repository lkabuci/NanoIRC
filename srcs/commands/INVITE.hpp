#pragma once

#ifndef __INVITE_HPP__
#define __INVITE_HPP__

#include "Command.hpp"

class INVITE : public Command {
  public:
    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);
};

#endif