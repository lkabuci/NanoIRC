#ifndef IRC_SRCS_BOT
#define IRC_SRCS_BOT

#include "../../include/IRC.h"
#include "../client/Client.hpp"
#include "../socket/AddressResolver.hpp"
#include "Timer.hpp"

class BOT {
  public:
    explicit BOT(const char* port);
    void       addToClients();
    const char execute();

  private:
    const AddressResolver _address;
};

#endif // IRC_SRCS_BOT
