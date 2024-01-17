#pragma once

#ifndef __NICK_HPP__
#define __NICK_HPP__

#include "Command.hpp"

class NICK : public Command {
  public:
    NICK();
    NICK(const NICK& nick);
    ~NICK();

    NICK& operator=(const NICK& nick);

    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);

    const std::string& getNickname() const;

  private:
    std::string _nick;
};

#endif
