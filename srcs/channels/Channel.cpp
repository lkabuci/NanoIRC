#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(const std::string& name, const std::string& password,
                 CHANNEL_MODE::Modes mode)
    : _password(password), _name(name), _mode(mode) {}

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

size_t Channel::getNumberOfMembers() const {
    return _members.size();
}

void Channel::sendToAll(const std::string& message) {
    std::map<Client*, MEMBER_PERMISSION>::iterator it = _members.begin();

    for (; it != _members.end(); ++it)
        send(it->first->getSockfd(), message.c_str(), message.length(), 0);
}

void Channel::remove(const std::string& username) {
    std::map<Client*, MEMBER_PERMISSION>::iterator it = _members.begin();

    for (; it != _members.end(); ++it) {
        if (it->first->getUserInfo().getUsername() == username) {
            _members.erase(it);
            return;
        }
    }
    throw std::runtime_error("No such client.");
}

void Channel::remove(Client* client) {
    std::map<Client*, MEMBER_PERMISSION>::iterator client_pos =
        _members.find(client);

    if (client_pos == _members.end())
        throw std::runtime_error("No such client.");
    _members.erase(client_pos);
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

bool Channel::modeIsSet(CHANNEL_MODE::Modes mode) {
    return _mode & mode;
}

bool Channel::flagIsSet(Client* client, MEMBER_PERMISSION flag) {
    std::map<Client*, MEMBER_PERMISSION>::iterator it = _members.find(client);

    return (it == _members.end()) ? false : it->second & flag;
}

Client* Channel::getClient(const std::string& username) {
    std::map<Client*, MEMBER_PERMISSION>::iterator it = _members.begin();

    for (; it != _members.end(); ++it) {
        if (it->first->getUserInfo().getUsername() == username)
            return it->first;
    }
    return NULL;
}

const std::string& Channel::getPassword() const {
    return _password;
}

void Channel::setPassword(const std::string& password) {
    _password = password;
}

CHANNEL_MODE::Modes operator|(CHANNEL_MODE::Modes a, CHANNEL_MODE::Modes b) {
    return static_cast<CHANNEL_MODE::Modes>(static_cast<int>(a) |
                                            static_cast<int>(b));
}

CHANNEL_MODE::Modes operator&(CHANNEL_MODE::Modes a, CHANNEL_MODE::Modes b) {
    return static_cast<CHANNEL_MODE::Modes>(static_cast<int>(a) &
                                            static_cast<int>(b));
}
