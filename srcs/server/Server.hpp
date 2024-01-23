//
// Created by Redouane Elkaboussi on 1/14/24.
//

#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include "../../include/IRC.h"
#include "../client/Client.hpp"
#include "../socket/Socket.hpp"

class Server {
  public:
    explicit Server(const char* port, const char* passwd);
    ~Server();
    void                      run();
    static const std::string& getPasswd();

  private:
    Socket             _sock;
    const char*        _port;
    static std::string _passwd;
};

#endif // IRC_SERVER_HPP
