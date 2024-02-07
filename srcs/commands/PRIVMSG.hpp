#pragma once

#ifndef __PRIVMSG_HPP__
#define __PRIVMSG_HPP__

#include "Command.hpp"

class PRIVMSG : public Command {
  public:
    PRIVMSG();
    virtual ~PRIVMSG();

    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);

  private:
    PRIVMSG(const PRIVMSG&);
    PRIVMSG& operator=(const PRIVMSG&);
};

#endif