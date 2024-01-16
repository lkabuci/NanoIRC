//
// Created by Redouane Elkaboussi on 1/16/24.
//

#include "UserInfo.hpp"

const std::string& UserInfo::getUsername() const {
    return _username;
}

void UserInfo::setUsername(const std::string& username) {
    _username = username;
}

const std::string& UserInfo::getNickname() const {
    return _nickname;
}

void UserInfo::setNickname(const std::string& nickname) {
    _nickname = nickname;
}

const std::string& UserInfo::getRealname() const {
    return _realname;
}

void UserInfo::setRealname(const std::string& realname) {
    _realname = realname;
}
