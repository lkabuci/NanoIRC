#include "JOIN.hpp"

JOIN::JOIN() : _sender(NULL) {}

JOIN::JOIN(const JOIN& join)
    : _channels(join._channels), _keys(join._keys), _sender(join._sender) {}

JOIN::~JOIN() {}

JOIN& JOIN::operator=(const JOIN& join) {
    if (this == &join)
        return *this;
    _channels = join._channels;
    _keys = join._keys;
    _sender = join._sender;
    return *this;
}

void JOIN::execute(Client* client, const std::vector<std::string>& parameters) {
    if (parameters.empty()) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_NEEDMOREPARAMS,
                     "JOIN");
        return;
    }
    if (!_userIsRegistered())
        return;
    _sender = client;
    Parser::init(Utils::join(parameters));
    _setChannels();
    _setKeys();
    _joinUser();
}

bool JOIN::_userIsRegistered() {
    if (_sender->getUserInfo().isRegistered())
        return true;
    Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_NOTREGISTERED, "");
    return false;
}

void JOIN::_joinUser() {
    for (size_t i = 0; i < _channels.size(); ++i) {
        if (!TChannels::exist(_channels[i]))
            _createChannel(i);
        else {
            _addToChannel(TChannels::channel(_channels[i]), i);
        }
    }
}

void JOIN::_createChannel(const size_t& index) {
    Channel channel(_channels[index]);

    if (index < _keys.size()) {
        channel.setPassword(_keys[index]);
        channel.setMode(CHANNEL_MODE::SET_KEY);
    }
    channel.add(_sender, MEMBER_PERMISSION::OPERATOR);
    TChannels::add(_channels[index], channel);
    _sendSuccessReply(_channels[index]);
}

void JOIN::_sendSuccessReply(const std::string& name) {
    std::string msg = name + " " + _sender->getUserInfo().getNickname() +
                      " has joined (~" + _sender->getUserInfo().getNickname() +
                      "@" + Reactor::getInstance().getServerIp() + ")";

    Reply::sendn(_sender->getSockfd(), msg);
}

void JOIN::_addToChannel(Channel& channel, const size_t& index) {
    if (channel.exist(_sender))
        return;
    if (_channelIsInviteOnly(channel) && !channel.isInvited(_sender)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_INVITEONLYCHAN,
                     channel.name());
        return;
    }
    if (_channelHasKey(channel) && !_keyIsCorrect(channel, index))
        return;
    _addClientToChannel(channel, MEMBER_PERMISSION::REGULAR);
}

void JOIN::_addClientToChannel(Channel&                 channel,
                               MEMBER_PERMISSION::Flags flag) {
    channel.add(_sender, flag);
    if (channel.isInvited(_sender))
        channel.eraseFromInviteeslist(_sender);
}

void JOIN::_setChannels() {
    if (!Parser::match(TYPES::HASH)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     "JOIN");
        throw std::exception();
    }
    _channels.push_back(Parser::advance().lexeme());
    while (!Parser::isAtEnd() && Parser::match(TYPES::COMMA))
        _addChannel();
}

void JOIN::_addChannel() {
    if (!Parser::match(TYPES::HASH)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     "JOIN");
        throw std::exception();
    }
    if (Parser::isAtEnd()) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     "JOIN");
        throw std::exception();
    }
    _channels.push_back(Parser::advance().lexeme());
}

bool JOIN::_keyIsCorrect(Channel& channel, const size_t& index) {
    if (index < _keys.size() && _keys[index] == channel.getPassword())
        return true;
    Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_BADCHANNELKEY,
                 channel.name());
    return false;
}

bool JOIN::_channelHasKey(Channel& channel) {
    return channel.modeIsSet(CHANNEL_MODE::SET_KEY);
}

void JOIN::_setKeys() {
    if (Parser::isAtEnd())
        return;
    if (Parser::match(TYPES::SPACE)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     "JOIN");
        throw std::exception();
    }
    _keys.push_back(Parser::advance().lexeme());
    while (!Parser::isAtEnd())
        _addKey();
}

void JOIN::_addKey() {
    if (Parser::match(TYPES::COMMA)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     "JOIN");
        throw std::exception();
    }
    if (Parser::isAtEnd()) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     "JOIN");
        throw std::exception();
    }
    _keys.push_back(Parser::advance().lexeme());
}

bool JOIN::_channelIsInviteOnly(Channel& channel) {
    return channel.modeIsSet(CHANNEL_MODE::SET_INVITE_ONLY);
}
