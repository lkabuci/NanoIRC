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

    bool _nicknameAlreadyExists(Client* client);
    bool _notEnoughParams(Client*                         client,
                          const std::vector<std::string>& parameters);
    bool _userSetPassword(Client* client);
    bool _setNickname(Client* client, const std::string& param);
    void _welcomeUser(Client* client);
};

//  <nick>       ::= <letter> { <letter> | <number> | <special> }
//  <letter>     ::= 'a' ... 'z' | 'A' ... 'Z'
//  <number>     ::= '0' ... '9'
//  <special>    ::= '-' | '[' | ']' | '\' | '`' | '^' | '{' | '}'

#endif
