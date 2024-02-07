#pragma once

#ifndef __TOPIC_HPP__
#define __TOPIC_HPP__

#include "Command.hpp"

class TOPIC : public Command {
  public:
    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);
};
#endif