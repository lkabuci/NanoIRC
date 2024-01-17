//
// Created by Redouane Elkaboussi on 1/16/24.
//

#include "UserInfo.hpp"

const std::string& UserInfo::getUsername() const {
    return _username;
}

void UserInfo::setUsername(const std::string& username) {
    _username = username;
    _flags |= USER_SET;
}

const std::string& UserInfo::getNickname() const {
    return _nickname;
}

void UserInfo::setNickname(const std::string& nickname) {
    _nickname = nickname;
    _flags |= NICK_SET;
}

const std::string& UserInfo::getRealname() const {
    return _realname;
}

void UserInfo::setRealname(const std::string& realname) {
    _realname = realname;
}

const std::string&  UserInfo::getPassword() const {
    return _password;
}

void UserInfo::setPassword(const std::string& password) {
    _password = password;
    _flags |= PASSWORD_SET;
}

bool    UserInfo::isSet(int flag) {
    return _flags & flag;
}

bool    UserInfo::isRegistered() {
    return _flags & (PASSWORD_SET | NICK_SET | USER_SET);
}
