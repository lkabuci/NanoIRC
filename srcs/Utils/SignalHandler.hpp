#pragma once
#include "../../include/IRC.h"

class SignalHandler {
  public:
    SignalHandler();

  private:
    static void handleSIGINT(int signum);
};
