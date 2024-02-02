#pragma once

#ifndef __QUIT_HPP__
#define __QUIT_HPP__

#include "Command.hpp"

class QUIT : public Command {
  public:
    explicit QUIT();
    virtual ~QUIT();

    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);

  private:
    QUIT(const QUIT&);
    QUIT& operator=(const QUIT&);

    std::string _message;
    Client*     _client;

    void _setQuitMessage();
    void _sendReply();
};

#endif
