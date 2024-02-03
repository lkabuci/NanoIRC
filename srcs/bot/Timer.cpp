#include "Timer.hpp"
#include <cstring>

char Timer::_buffer[];

Timer::Timer() : _start(std::time(NULL)) {
    _start = std::time(NULL);
}

const char* Timer::getTime() const {
    return format(std::time(NULL));
}

const char* Timer::format(std::time_t currentTime) const {
    std::tm* localTime = std::localtime(&currentTime);
    std::memset(_buffer, 0, sizeof(_buffer));
    std::strftime(_buffer, sizeof(_buffer), "%d/%m/%Y %H:%M:%S", localTime);
    return _buffer;
}
