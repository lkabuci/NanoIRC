//
// Created by Redouane Elkaboussi on 1/14/24.
//

#ifndef IRC_CLIENT_HPP
#define IRC_CLIENT_HPP

#include "../../include/IRC.h"
#include "UserInfo.hpp"

#define PORT_LEN 6

class Client {
  public:
    explicit Client(struct sockaddr_storage&, int);
    ~Client();
    int                getSockfd() const;
    UserInfo&    getUserInfo();
    void               setUserInfo(const UserInfo& userInfo);
    const std::string& getMessage() const;
    void               appendMessage(const std::string& message);
    bool               isDoneReading() const;
    void               setIsDoneReading(bool isDoneReading);

  private:
    struct sockaddr_storage& _sockAddr;
    int                      _sockfd;
    char                     _ip[INET6_ADDRSTRLEN];
    char                     _port[PORT_LEN];
    UserInfo                 _userInfo;
    void                     fillClientIpPort();
    bool                     _isDoneReading;
    std::string              _message;
};

#endif // IRC_CLIENT_HPP
