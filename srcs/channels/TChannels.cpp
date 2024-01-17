#include "TChannels.hpp"

std::map<std::string, Channel> TChannels::_channels;

size_t TChannels::size() {
    return _channels.size();
}

void TChannels::remove(const std::string& name) {
    std::map<std::string, Channel>::const_iterator channel_pos =
        _channels.find(name);

    if (channel_pos == _channels.end())
        throw std::runtime_error("No such channel.");
    _channels.erase(channel_pos);
}

Channel& TChannels::channel(const std::string& name) {
    std::map<std::string, Channel>::iterator channel_pos = _channels.find(name);

    if (channel_pos == _channels.end())
        throw std::runtime_error("No such channel.");
    return channel_pos->second;
}

void TChannels::create(const std::string& name, Client* client,
                       MEMBER_PERMISSION premission) {
    Channel channel(name);

    channel.add(client, premission);
    _channels[name] = channel;
}

void TChannels::add(const std::string& name, Channel channel) {
    _channels[name] = channel;
}

void TChannels::removeMember(const std::string& channel_name,
                             const std::string& nickname) {
    std::map<std::string, Channel>::iterator channel_pos =
        _channels.find(channel_name);

    if (channel_pos == _channels.end())
        throw std::runtime_error("No such channel.");
    channel_pos->second.remove(nickname);
    if (channel_pos->second.empty())
        _channels.erase(channel_pos);
}

bool TChannels::exist(const std::string& name) {
    std::map<std::string, Channel>::const_iterator it = _channels.find(name);

    return it != _channels.end();
}
