#pragma once

#ifndef __PASS_HPP__
#define __PASS_HPP__

#include "Command.hpp"

class PASS : public Command {
  public:
    PASS();
    PASS(const PASS& p);
    virtual ~PASS();

    PASS& operator=(const PASS& p);

    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);

  private:
    std::string _password;

    bool _validParameters(Client*                         client,
                          const std::vector<std::string>& parameters);
    void _sendErrorReply(Client* client);
};

#endif