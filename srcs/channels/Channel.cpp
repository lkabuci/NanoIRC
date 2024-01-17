#include "Channel.hpp"

Channel::Channel(const std::string& name)
    : _name(name), _mode(CHANNEL_MODE::SET_KEY) {}

void Channel::add(Client* newMember, MEMBER_PERMISSION premission) {
    _members.insert(
        std::pair<Client*, MEMBER_PERMISSION>(newMember, premission));
}

void Channel::setMode(CHANNEL_MODE::Modes mode) {
    _mode = _mode | mode;
}

void Channel::updateMode(CHANNEL_MODE::Modes mode) {
    _mode = mode;
}

const size_t Channel::getNumberOfMembers() const {
    return _members.size();
}

void Channel::sendToAll(const std::string& message) {
    std::map<Client*, MEMBER_PERMISSION>::iterator it = _members.begin();

    for (; it != _members.end(); ++it)
        send(it->first->getSockfd(), message.c_str(), message.length(), 0);
}

void Channel::remove(const std::string& nickname) {
    std::map<Client*, MEMBER_PERMISSION>::iterator it = _members.begin();

    for (; it != _members.end(); ++it) {
        if (it->first->getUserInfo().getNickname() == nickname) {
            _members.erase(it);
            return;
        }
    }
    throw std::runtime_error("No such client.");
}

void Channel::clear() {
    _members.clear();
}

bool Channel::empty() const {
    return _members.empty();
}

const std::string& Channel::name() const {
    return _name;
}

bool Channel::isSet(CHANNEL_MODE::Modes mode) {
    return _mode & mode;
}

Client* Channel::getClient(const std::string& nickname) {
    std::map<Client*, MEMBER_PERMISSION>::iterator it = _members.begin();

    for (; it != _members.end(); ++it) {
        if (it->first->getUserInfo().getNickname() == nickname)
            return it->first;
    }
    return NULL;
}

CHANNEL_MODE::Modes operator|(CHANNEL_MODE::Modes a, CHANNEL_MODE::Modes b) {
    return static_cast<CHANNEL_MODE::Modes>(static_cast<int>(a) |
                                            static_cast<int>(b));
}

CHANNEL_MODE::Modes operator&(CHANNEL_MODE::Modes a, CHANNEL_MODE::Modes b) {
    return static_cast<CHANNEL_MODE::Modes>(static_cast<int>(a) &
                                            static_cast<int>(b));
}
