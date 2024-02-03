#include "../../include/IRC.h"
#include <ctime>

class Timer {
  public:
    Timer();
    const char* getTime() const;

  private:
    static char _buffer[20];
    std::time_t _start;
    const char* format(std::time_t currentTime) const;
};
