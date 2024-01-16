#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(
    const CHANNEL_MODES& mode /*, const std::vector<Client>& clients*/)
    : _mode(mode) /*, _clients(clients)*/ {}

Channel::Channel(const Channel& channel)
    : _mode(channel._mode) /*, _clients(channel._clients)*/ {}

Channel::~Channel() {}

Channel& Channel::operator=(const Channel& channel) {
    if (this == &channel)
        return *this;
    _mode = channel._mode;
    //_clients = channel._clients;
    return *this;
}

const CHANNEL_MODES& Channel::get_mode() const {
    return _mode;
}

// const Client& Channel::get_client() const {
//     return _clients;
// }

void Channel::set_channel_mode(CHANNEL_MODES mode) {
    _mode = _mode | mode;
}

void Channel::update_channel_mode(CHANNEL_MODES mode) {
    _mode = mode;
}

// void Channel::add_client(const Client* client, USER_MODES mode) {
//     _clients[client] = mode;
// }

CHANNEL_MODES operator|(CHANNEL_MODES a, CHANNEL_MODES b) {
    return static_cast<CHANNEL_MODES>(static_cast<int>(a) |
                                      static_cast<int>(b));
}

bool operator&(CHANNEL_MODES a, CHANNEL_MODES b) {
    return (static_cast<int>(a) & static_cast<int>(b));
}
