#pragma once

#include "../include/IRC.h"

class ServerHelper {
  public:
    static const char *GetIPAddressFromSockAddr(int);
    static const char *GetPortAddressFromSockAddr(int);
};
