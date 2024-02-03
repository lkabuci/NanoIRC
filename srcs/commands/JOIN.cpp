#include "JOIN.hpp"

JOIN::JOIN() : _sender(NULL) {}

JOIN::~JOIN() {}

//! TODO check limit

void JOIN::execute(Client* client, const std::vector<std::string>& parameters) {
    if (parameters.empty()) {
        Reply::errNotEnoughParams(client, "JOIN");
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
    try {
        _setChannels();
        _setKeys();
        _joinUser();
    } catch (const std::exception& e) {
    }
}

void JOIN::_leaveAllChannels() {
    while (TChannels::userExists(_sender->getUserInfo().getNickname())) {
        Channel& channel =
            TChannels::userChannel(_sender->getUserInfo().getNickname());

        _sendLeftReply(channel);
        channel.remove(_sender);
    }
}

void JOIN::_sendLeftReply(Channel& channel) {
    std::string reply = ":" + _sender->getUserInfo().getNickname() + "!~" +
                        _sender->getUserInfo().getUsername() + "@" +
                        _sender->getIp() + " PART " + channel.name() +
                        " :Left all channels\r\n";

    channel.sendToAll(_sender, reply);
    send(_sender->getSockfd(), reply.c_str(), reply.length(), 0);
}

bool JOIN::_userIsRegistered() {
    if (_sender->getUserInfo().isRegistered())
        return true;
    std::string reply = std::string(":") + Reactor::getServerName() + " 451 ";

    if (_sender->getUserInfo().getNickname().empty()) {
        reply.append("*  ");
    } else {
        reply.append(_sender->getUserInfo().getNickname() + " " +
                     _sender->getUserInfo().getUsername());
    }
    reply.append(" :You have not registered\r\n");
    send(_sender->getSockfd(), reply.c_str(), reply.length(), 0);
    return false;
}

void JOIN::_joinUser() {
    for (size_t i = 0; i < _channels.size(); ++i) {
        if (!_validChannel(_channels[i]))
            continue;
        if (!TChannels::exist(_channels[i]))
            _createChannel(i);
        else {
            _addToChannel(TChannels::channel(_channels[i]), i);
        }
    }
}

bool JOIN::_validChannel(const std::string& channel) {
    if (channel[0] == '#') {
        return true;
    }
    Reply::errNoSuchChannel(_sender, channel);
    return false;
}

void JOIN::_createChannel(const size_t& index) {
    Channel channel(_channels[index]);

    channel.add(_sender, MEMBER_PERMISSION::OPERATOR);
    TChannels::add(_channels[index], channel);
    Reply::channelReply(_sender, _channels[index]);
}

void JOIN::_addToChannel(Channel& channel, const size_t& index) {
    if (channel.exist(_sender))
        return;
    if (_channelIsInviteOnly(channel)) {
        if (!channel.isInvited(_sender)) {
            Reply::errInviteOnlyChan(_sender, channel.name());
            return;
        }
        _joinWithoutAsk(channel);
        return;
    }
    if (channel.getNumberOfMembers() + 1 > channel.getLimit()) {
        Reply::errChannelIsFull(_sender, channel.name());
        return;
    }
    if (_channelHasKey(channel) && !_keyIsCorrect(channel, index))
        return;
    _addClientToChannel(channel, MEMBER_PERMISSION::REGULAR);
    _addToChannelReply(channel);
    _tellMembers(channel);
}

void JOIN::_addToChannelReply(Channel& channel) {
    std::string msg1 = ":" + _sender->getUserInfo().getNickname() + "!~" +
                       _sender->getUserInfo().getUsername() + "@" +
                       _sender->getIp() + " JOIN " + channel.name() + CR_LF;
    std::string msg2 =
        std::string(":") + Reactor::getServerName() + " 353 " +
        _sender->getUserInfo().getNickname() + " = " + channel.name() + " :" +
        _sender->getUserInfo().getNickname() + _getMembersList(channel) + CR_LF;
    std::string msg3 = std::string(":") + Reactor::getServerName() + " 366 " +
                       _sender->getUserInfo().getNickname() + " " +
                       channel.name() + " :End of /Names list.\r\n";

    send(_sender->getSockfd(), msg1.c_str(), msg1.length(), 0);
    send(_sender->getSockfd(), msg2.c_str(), msg2.length(), 0);
    send(_sender->getSockfd(), msg3.c_str(), msg3.length(), 0);
}

std::string JOIN::_getMembersList(Channel& channel) {
    std::map<Client*, MEMBER_PERMISSION::Flags> members = channel.getMembers();
    std::map<Client*, MEMBER_PERMISSION::Flags>::const_iterator it;
    std::string                                                 list;

    for (it = members.begin(); it != members.end(); ++it) {
        if (it->first->getUserInfo().getNickname() ==
            _sender->getUserInfo().getNickname())
            continue;
        if (it->second == MEMBER_PERMISSION::OPERATOR)
            list.append(" @" + it->first->getUserInfo().getNickname());
        else
            list.append(" " + it->first->getUserInfo().getNickname());
    }
    return list;
}

void JOIN::_joinWithoutAsk(Channel& channel) {
    _addClientToChannel(channel, MEMBER_PERMISSION::REGULAR);
    _addToChannelReply(channel);
    Reply::channelReply(_sender, channel.name());
    _tellMembers(channel);
    _addClientToChannel(channel, MEMBER_PERMISSION::REGULAR);
    _addToChannelReply(channel);
    _tellMembers(channel);
}

void JOIN::_tellMembers(Channel& channel) {
    std::string msg = ":" + _sender->getUserInfo().getNickname() + "!~" +
                      _sender->getUserInfo().getUsername() + "@" +
                      _sender->getIp() + " JOIN " + channel.name() + CR_LF;

    channel.sendToAll(_sender, msg);
}

void JOIN::_addClientToChannel(Channel&                 channel,
                               MEMBER_PERMISSION::Flags flag) {
    channel.add(_sender, flag);
    if (channel.isInvited(_sender))
        channel.eraseFromInviteeslist(_sender);
}

void JOIN::_setChannels() {
    do {
        _addChannel();
    } while (!_isEnd());
}

void JOIN::_addChannel() {
    if (Parser::isAtEnd() || Parser::check(TYPES::SPACE))
        return;
    std::string channel = Parser::advance().lexeme();

    if (!Parser::isAtEnd() && !Parser::check(TYPES::COMMA)) {
        channel.append(Parser::advance().lexeme());
    }
    _channels.push_back(channel);
}

bool JOIN::_keyIsCorrect(Channel& channel, const size_t& index) {
    if (index < _keys.size() && _keys[index] == channel.getPassword())
        return true;
    Reply::errBadChannelKey(_sender, channel.name());
    return false;
}

bool JOIN::_channelHasKey(Channel& channel) {
    return channel.modeIsSet(CHANNEL_MODE::SET_KEY);
}

void JOIN::_setKeys() {
    Parser::match(TYPES::SPACE);
    if (Parser::isAtEnd())
        return;
    do {
        _addKey();
    } while (!_isEnd());
}

void JOIN::_addKey() {
    if (Parser::check(TYPES::COMMA)) {
        _keys.push_back("");
    } else {
        // std::string key;

        // while (!_isEnd()) {
        //     if (Parser::check(TYPES::SPACE))
        //         break;
        //     key.append(Parser::advance().lexeme());
        // }
        _keys.push_back(Parser::advance().lexeme());
    }
}

bool JOIN::_channelIsInviteOnly(Channel& channel) {
    return channel.modeIsSet(CHANNEL_MODE::SET_INVITE_ONLY);
}

bool JOIN::_isEnd() {
    return Parser::isAtEnd() || !Parser::match(TYPES::COMMA) ||
           Parser::check(TYPES::SPACE);
}
