#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(const std::string& name, const std::string& password,
                 CHANNEL_MODE::Modes mode)
    : _name(name), _password(password), _mode(mode) {}

Channel::Channel(const Channel& channel)
    : _name(channel._name), _password(channel._password),
      _members(channel._members), _invited(channel._invited),
      _mode(channel._mode), _topic(channel._topic) {}

Channel& Channel::operator=(const Channel& channel) {
    if (this == &channel)
        return *this;
    _name = channel._name;
    _password = channel._password;
    _members = channel._members;
    _invited = channel._invited;
    _mode = channel._mode;
    _topic = channel._topic;
    return *this;
}

void Channel::add(Client* newMember, MEMBER_PERMISSION::Flags premission) {
    _members.insert(
        std::pair<Client*, MEMBER_PERMISSION::Flags>(newMember, premission));
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
    std::map<Client*, MEMBER_PERMISSION::Flags>::iterator it = _members.begin();

    for (; it != _members.end(); ++it)
        send(it->first->getSockfd(), message.c_str(), message.length(), 0);
}

void Channel::remove(const std::string& username) {
    std::map<Client*, MEMBER_PERMISSION::Flags>::iterator it = _members.begin();

    for (; it != _members.end(); ++it) {
        if (it->first->getUserInfo().getUsername() == username) {
            _members.erase(it);
            return;
        }
    }
    throw std::runtime_error("No such client.");
}

void Channel::remove(Client* client) {
    std::map<Client*, MEMBER_PERMISSION::Flags>::iterator client_pos =
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

bool Channel::exist(Client* client) {
    return _members.find(client) != _members.end();
}

const std::string& Channel::name() const {
    return _name;
}

bool Channel::modeIsSet(CHANNEL_MODE::Modes mode) {
    return _mode & mode;
}

bool Channel::flagIsSet(Client* client, MEMBER_PERMISSION::Flags flag) {
    std::map<Client*, MEMBER_PERMISSION::Flags>::iterator it =
        _members.find(client);

    return (it == _members.end()) ? false : it->second & flag;
}

Client* Channel::getClient(const std::string& username) {
    std::map<Client*, MEMBER_PERMISSION::Flags>::iterator it = _members.begin();

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

void Channel::invite(Client* client) {
    _invited.push_back(client);
}

bool Channel::isInvited(Client* client) {
    return std::find(_invited.begin(), _invited.end(), client) !=
           _invited.end();
}

const std::string& Channel::getTopic() const {
    return _topic;
}

void Channel::setTopic(const std::string& topic) {
    _topic = topic;
}

CHANNEL_MODE::Modes operator|(CHANNEL_MODE::Modes a, CHANNEL_MODE::Modes b) {
    return static_cast<CHANNEL_MODE::Modes>(static_cast<int>(a) |
                                            static_cast<int>(b));
}

CHANNEL_MODE::Modes operator&(CHANNEL_MODE::Modes a, CHANNEL_MODE::Modes b) {
    return static_cast<CHANNEL_MODE::Modes>(static_cast<int>(a) &
                                            static_cast<int>(b));
}
