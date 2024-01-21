#pragma once

#ifndef __USER_HPP__
#define __USER_HPP__

#include "Command.hpp"

class USER : public Command {
  public:
    USER();
    USER(const USER& user);
    virtual ~USER();

    USER& operator=(const USER& user);

    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);

  private:
    std::string _username;
    std::string _realname;

    void _parseRealName(Client* client);
    bool _notEnoughParams(Client*                         client,
                          const std::vector<std::string>& parameters);
    bool _userSetPassword(Client* client);
    void _ignoreHostAndServerNames();
    void _setUserInfo(Client* client);
    void _welcomeUser(Client* client);
    void _oneParam(Client* client);
};

#endif
