#pragma once

#ifndef __NICK_HPP__
#define __NICK_HPP__

#include "Command.hpp"

class NICK : public Command {
  public:
    explicit NICK();
    ~NICK();

    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);

  private:
    NICK(const NICK& nick);
    NICK& operator=(const NICK& nick);

    std::string _nick;

    bool _nicknameAlreadyInUse(Client* client);
    bool _nickIsSame(Client* client);
    bool _notEnoughParams(Client*                         client,
                          const std::vector<std::string>& parameters);
    bool _userSetPassword(Client* client);
    void _newUser(Client* client);
    void _rpl(Client* client);
    void _change(Client* client);
    bool _nicknameChange(Client* client);
    void _welcome(Client* client);
    void _errNoNicknameGiven(Client* client);
    void _errNicknameAlreadyInUse(Client* client);
    void _errErroneousNickname(Client* client, const std::string& name);
};

#endif
