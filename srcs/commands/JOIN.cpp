#include "JOIN.hpp"

JOIN::JOIN() : _sender(NULL) {}

JOIN::~JOIN() {}

void JOIN::execute(Client* client, const std::vector<std::string>& parameters) {
    if (parameters.empty()) {
        //: adrift.sg.quakenet.org 461 i1 JOIN :Not enough parameters
        _errNotEnoughParams(client);
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
    TChannels::removeUserFromAll(_sender->getUserInfo().getNickname());
}

bool JOIN::_userIsRegistered() {
    if (_sender->getUserInfo().isRegistered())
        return true;
    std::string reply = ":localhost 451 ";

    if (_sender->getUserInfo().getNickname().empty()) {
        //: atw.hu.quakenet.org 451 *  :Register first.
        reply.append("*  ");
    } else {
        //: atw.hu.quakenet.org 451 i1 i1 :Register first.
        reply.append(_sender->getUserInfo().getNickname() + " " +
                     _sender->getUserInfo().getUsername());
    }
    reply.append(" :You have not registered\r\n");
    send(_sender->getSockfd(), reply.c_str(), reply.length(), 0);
    return false;
}

void JOIN::_joinUser() {
    if (_channels.empty()) {
        _errNotEnoughParams(_sender);
        return;
    }
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
    _errNoSuchChannel(channel);
    return false;
}

void JOIN::_createChannel(const size_t& index) {
    Channel channel(_channels[index]);

    channel.setLimit(4);
    channel.add(_sender, MEMBER_PERMISSION::OPERATOR);
    TChannels::add(_channels[index], channel);
    _channelReply(_channels[index]);
}

void JOIN::_addToChannel(Channel& channel, const size_t& index) {
    if (channel.exist(_sender))
        return;
    if (_channelIsInviteOnly(channel) && !channel.isInvited(_sender)) {
        _errInviteOnlyChan(channel.name());
        return;
    }
    if (index >= channel.getLimit()) {
        _errChannelIsFull(channel.name());
        return;
    }
    if (_channelHasKey(channel) && !_keyIsCorrect(channel, index))
        return;
    _addClientToChannel(channel, MEMBER_PERMISSION::REGULAR);
    _channelReply(channel.name());
    _tellMembers(channel);
}

void JOIN::_tellMembers(Channel& channel) {
    std::string msg = ":" + _sender->getUserInfo().getNickname() + "!~" +
                      _sender->getUserInfo().getUsername() + "@" +
                      Reactor::getInstance().getServerIp() + " JOIN " +
                      channel.name() + CR_LF;

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
    } while (!Parser::isAtEnd() && Parser::match(TYPES::COMMA));
}

void JOIN::_addChannel() {
    std::string channel = Parser::advance().lexeme();

    if (!Parser::isAtEnd() && !Parser::check(TYPES::COMMA)) {
        channel.append(Parser::advance().lexeme());
    }
    _channels.push_back(channel);
}

bool JOIN::_keyIsCorrect(Channel& channel, const size_t& index) {
    if (index < _keys.size() && _keys[index] == channel.getPassword())
        return true;
    //: hostsailor.ro.quakenet.org 475 i1 #ch2 :Cannot join channel, you need
    //: the correct key (+k)
    std::string reply =
        ":localhost 475 " + _sender->getUserInfo().getNickname() + " " +
        channel.name() +
        " :Cannot join channel, you need the correct key (+k)\r\n";

    send(_sender->getSockfd(), reply.c_str(), reply.length(), 0);
    return false;
}

bool JOIN::_channelHasKey(Channel& channel) {
    return channel.modeIsSet(CHANNEL_MODE::SET_KEY);
}

void JOIN::_setKeys() {
    if (Parser::isAtEnd())
        return;
    do {
        _addKey();
    } while (!Parser::isAtEnd() && Parser::match(TYPES::COMMA));
}

void JOIN::_addKey() {
    if (Parser::check(TYPES::COMMA)) {
        _keys.push_back("");
    } else {
        _keys.push_back(Parser::advance().lexeme());
    }
}

bool JOIN::_channelIsInviteOnly(Channel& channel) {
    return channel.modeIsSet(CHANNEL_MODE::SET_INVITE_ONLY);
}

//: nick1!~nick1@197.230.30.146 JOIN #ch1
//: euroserv.fr.quakenet.org 353 nick1 = #ch1 :@nick1
//: euroserv.fr.quakenet.org 366 nick1 #ch1 :End of /NAMES list.

void JOIN::_channelReply(const std::string& channel) {
    std::string msg1;
    std::string msg2;
    std::string msg3;

    msg1 = ":" + _sender->getUserInfo().getNickname() + "!" +
           _sender->getUserInfo().getUsername() + "@" +
           Reactor::getInstance().getServerIp() + " JOIN " + channel + CR_LF;

    msg2 = ":localhost 353 " + _sender->getUserInfo().getNickname() + " = " +
           channel + " :@" + _sender->getUserInfo().getNickname() + CR_LF;

    msg3 = ":localhost 366 " + _sender->getUserInfo().getNickname() + " " +
           channel + " :End of /NAMES list.\r\n";

    send(_sender->getSockfd(), msg1.c_str(), msg1.length(), 0);
    send(_sender->getSockfd(), msg2.c_str(), msg2.length(), 0);
    send(_sender->getSockfd(), msg3.c_str(), msg3.length(), 0);
}

void JOIN::_errNoSuchChannel(const std::string& name) {
    std::string reply = ":localhost 403 " +
                        _sender->getUserInfo().getNickname() + " " + name +
                        " :No such channel\r\n";

    send(_sender->getSockfd(), reply.c_str(), reply.length(), 0);
}

void JOIN::_errNotEnoughParams(Client* client) {
    std::string reply = ":localhost 461 " +
                        client->getUserInfo().getNickname() +
                        " JOIN :Not enough parameters\r\n";

    send(client->getSockfd(), reply.c_str(), reply.length(), 0);
}

void JOIN::_errInviteOnlyChan(const std::string& name) {
    //: hostsailor.ro.quakenet.org 473 i1 #ch3 :Cannot join channel, you must be
    //: invited (+i)
    std::string reply = ":localhost 473 " +
                        _sender->getUserInfo().getNickname() + " " + name +
                        " :Cannot join channel, you must be invited (+i)\r\n";

    send(_sender->getSockfd(), reply.c_str(), reply.length(), 0);
}

void JOIN::_errChannelIsFull(const std::string& name) {
    //: hostsailor.ro.quakenet.org 471 i1 #ch1 :Cannot join channel, Channel is
    //: full (+l)
    std::string msg = ":localhost 471 " + _sender->getUserInfo().getNickname() +
                      " " + name +
                      " :Cannot join channel, Channel is full (+l)\r\n";

    send(_sender->getSockfd(), msg.c_str(), msg.length(), 0);
}
