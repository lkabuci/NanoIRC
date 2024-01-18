#ifndef IRC_SRCS_BOT
#define IRC_SRCS_BOT

#include "../../include/IRC.h"
#include "../client/Client.hpp"
#include "../socket/AddressResolver.hpp"

class BOT {
  public:
    explicit BOT(const char* port);
    void addToClients();

  private:
    const char*           _port;
    const AddressResolver _address;
};

#endif // IRC_SRCS_BOT
