#pragma once

#ifndef __PASS_HPP__
#define __PASS_HPP__

#include "Command.hpp"

class PASS : public Command {
  public:
    explicit PASS();
    virtual ~PASS();

    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);

  private:
    PASS(const PASS& p);
    PASS& operator=(const PASS& p);

    std::string _password;

    bool _validParameters(Client*                         client,
                          const std::vector<std::string>& parameters);
    void _errNeedMoreParams(Client* client);
    void _errAlreadyRegistred(Client* client);
    void _errPasswdMismatch(Client* client);
};

#endif