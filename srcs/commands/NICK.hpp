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

    virtual void execute(const std::string& first,
                         const std::string& parameters);

    virtual const std::string& get() const;

  private:
    std::string _nick;
};

#endif