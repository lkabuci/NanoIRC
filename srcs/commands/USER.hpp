#pragma once

#ifndef __USER_HPP__
#define __USER_HPP__

#include "Command.hpp"

class USER : public Command {
  public:
    explicit USER();
    virtual ~USER();

    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);

  private:
    USER(const USER&);
    USER& operator=(const USER&);

    std::string _username;
    std::string _realname;

    void _errAlreadyRegistred(Client* client);
    void _parseRealName();
    bool _notEnoughParams(Client*                         client,
                          const std::vector<std::string>& parameters);
    bool _userSetPassword(Client* client);
    void _ignoreHostAndServerNames();
    void _setUserInfo(Client* client);
    void _errNotEnoughParams(Client* client);
    void _errErroneousNickname(Client* client, const std::string& name);
};

#endif
