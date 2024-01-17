#pragma once

#ifndef __MODE_HPP__
#define __MODE_HPP__

#include "Command.hpp"

class MODE : public Command {
  public:
    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);
};

#endif