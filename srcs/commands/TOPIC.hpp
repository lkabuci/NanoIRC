#pragma once

#ifndef __TOPIC_HPP__
#define __TOPIC_HPP__

#include "Command.hpp"

class TOPIC : public Command {
  public:
    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);
};
void sendSucc(int fd, const std::string& nick, const std::string& channel,
              const std::string& topic);
void sendSucc2(int fd, const std::string& nick, const std::string& channel,
               const std::string& nickSetter, std::string time);
#endif