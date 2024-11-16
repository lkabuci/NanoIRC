#include "ChannelMode.hpp"

ChannelMode::ChannelMode() {}

ChannelMode::ChannelMode(const CHANNEL_MODES& mode) : _mode(mode) {}

ChannelMode::ChannelMode(const ChannelMode& channel) : _mode(channel._mode) {}

ChannelMode::~ChannelMode() {}

ChannelMode& ChannelMode::operator=(const ChannelMode& channel) {
    if (this == &channel)
        return *this;
    _mode = channel._mode;
    return *this;
}

const CHANNEL_MODES& ChannelMode::getMode() const {
    return _mode;
}

void ChannelMode::setChannelMode(CHANNEL_MODES mode) {
    _mode = _mode | mode;
}

void ChannelMode::updateChannelMode(CHANNEL_MODES mode) {
    _mode = mode;
}

CHANNEL_MODES operator|(CHANNEL_MODES a, CHANNEL_MODES b) {
    return static_cast<CHANNEL_MODES>(static_cast<int>(a) |
                                      static_cast<int>(b));
}

bool operator&(CHANNEL_MODES a, CHANNEL_MODES b) {
    return (static_cast<int>(a) & static_cast<int>(b));
}
