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

  private:
    std::string _nick;

    bool _isNicknameCollision(Client* client);
    void _removeInstances();
    bool _notEnoughParams(Client*                         client,
                          const std::vector<std::string>& parameters);
    bool _userSetPassword(Client* client);
    void _sendErrNicknameCollision(Client* client);
    void _welcomeUser(Client* client);
};

#endif
