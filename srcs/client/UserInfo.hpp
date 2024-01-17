//
// Created by Redouane Elkaboussi on 1/16/24.
//

#ifndef IRC_USERINFO_HPP
#define IRC_USERINFO_HPP

#include "../../include/IRC.h"

class UserInfo {
  public:
    const std::string& getUsername() const;
    void               setUsername(const std::string& username);
    const std::string& getNickname() const;
    void               setNickname(const std::string& nickname);
    const std::string& getRealname() const;
    void               setRealname(const std::string& realname);
    const std::string& getPassword() const;
    void               setPassword(const std::string& password);
    bool               isSet(int flag);
    bool               isRegistered();

  private:
    enum    {PASSWORD_SET = 0, NICK_SET = 2, USER_SET = 4};

    std::string _password;
    std::string _username;
    std::string _nickname;
    std::string _realname;
    int         _flags;
};

#endif // IRC_USERINFO_HPP
