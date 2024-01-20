#include "JOIN.hpp"

JOIN::JOIN() : _sender(NULL) {}

JOIN::~JOIN() {}

void JOIN::execute(Client* client, const std::vector<std::string>& parameters) {
    if (parameters.empty()) {
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_NEEDMOREPARAMS,
                     client->getUserInfo().getNickname(), "JOIN");
        return;
    }
    _sender = client;
    if (!_userIsRegistered())
        return;
    if (parameters.size() == 1 && parameters[0] == "0") {
        _leaveAllChannels();
        return;
    }
    Parser::init(Utils::join(parameters));
    _setChannels();
    _setKeys();
    _joinUser();
}

void JOIN::_leaveAllChannels() {
    TChannels::removeUserFromAll(_sender->getUserInfo().getNickname());
}

bool JOIN::_userIsRegistered() {
    if (_sender->getUserInfo().isRegistered())
        return true;
    Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_NOTREGISTERED,
                 _sender->getUserInfo().getNickname(), "");
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
    Reply::rpl_topic(_sender->getSockfd(), _sender->getUserInfo().getNickname(),
                     _channels[index], "");
}

void JOIN::_addToChannel(Channel& channel, const size_t& index) {
    if (channel.exist(_sender))
        return;
    if (_channelIsInviteOnly(channel) && !channel.isInvited(_sender)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_INVITEONLYCHAN,
                     _sender->getUserInfo().getNickname(), channel.name());
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
                     _sender->getUserInfo().getNickname(), "JOIN");
        throw std::exception();
    }
    _channels.push_back(Parser::previous().lexeme() +
                        Parser::advance().lexeme());
    while (!Parser::isAtEnd() && Parser::match(TYPES::COMMA))
        _addChannel();
}

void JOIN::_addChannel() {
    if (!Parser::match(TYPES::HASH)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     _sender->getUserInfo().getNickname(), "JOIN");
        throw std::exception();
    }
    if (Parser::isAtEnd()) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     _sender->getUserInfo().getNickname(), "JOIN");
        throw std::exception();
    }
    _channels.push_back(Parser::previous().lexeme() +
                        Parser::advance().lexeme());
}

bool JOIN::_keyIsCorrect(Channel& channel, const size_t& index) {
    if (index < _keys.size() && _keys[index] == channel.getPassword())
        return true;
    Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_BADCHANNELKEY,
                 _sender->getUserInfo().getNickname(), channel.name());
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
                     _sender->getUserInfo().getNickname(), "JOIN");
    }
    _keys.push_back(Parser::advance().lexeme());
    while (!Parser::isAtEnd())
        _addKey();
}

void JOIN::_addKey() {
    if (Parser::match(TYPES::COMMA)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     _sender->getUserInfo().getNickname(), "JOIN");
        return;
    }
    if (Parser::isAtEnd()) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     _sender->getUserInfo().getNickname(), "JOIN");
        return;
    }
    _keys.push_back(Parser::advance().lexeme());
}

bool JOIN::_channelIsInviteOnly(Channel& channel) {
    return channel.modeIsSet(CHANNEL_MODE::SET_INVITE_ONLY);
}
