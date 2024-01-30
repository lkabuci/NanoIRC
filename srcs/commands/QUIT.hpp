#pragma once

#ifndef __QUIT_HPP__
#define __QUIT_HPP__

#include "Command.hpp"

class QUIT : public Command {
  public:
    QUIT();
    virtual ~QUIT();

    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);

  private:
    std::string _message;
    Client*     _client;

    void _setQuitMessage();
    void _sendReply();
};

#endif
