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
                     "JOIN", Reactor::getInstance().getServerIp());
        return;
    }
    if (!_userIsRegistered())
        return;
    _sender = client;
    Parser::init(Utils::join(parameters));
    _setChannels();
    _setKeys();
    _joinUser(client);
}

bool JOIN::_userIsRegistered() {
    if (!_sender->getUserInfo().isRegistered()) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_NOTREGISTERED, "",
                     Reactor::getInstance().getServerIp());
        return false;
    }
    return true;
}

void JOIN::_joinUser(Client* client) {
    for (size_t i = 0; i < _channels.size(); ++i) {
        if (!TChannels::exist(_channels[i]))
            _createChannel(client, i);
        else {
            _addToChannel(client, TChannels::channel(_channels[i]), i);
        }
    }
}

void JOIN::_createChannel(Client* client, const size_t& index) {
    Channel channel(_channels[index]);
    if (index < _keys.size()) {
        channel.setPassword(_keys[index]);
        channel.setMode(CHANNEL_MODE::SET_KEY);
    }
    channel.add(client, MEMBER_PERMISSION::OPERATOR);
    TChannels::add(_channels[index], channel);
}

void JOIN::_addToChannel(Client* client, Channel& channel,
                         const size_t& index) {
    if (channel.exist(client))
        return;
    if (_channelIsInviteOnly(channel) && !channel.isInvited(client))
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_INVITEONLYCHAN,
                     channel.name(), Reactor::getInstance().getServerIp());
    if (!_keyIsCorrect(channel, index))
        Reply::error(client->getSockfd(), ERROR_CODES::ERR_BADCHANNELKEY,
                     channel.name(), Reactor::getInstance().getServerIp());
    channel.add(client, MEMBER_PERMISSION::REGULAR);
    TChannels::add(_channels[index], channel);
    if (channel.isInvited(client))
        channel.eraseFromInviteeslist(client);
}

void JOIN::_setChannels() {
    // Parser::consume(TYPES::HASH, "channel must begin with #.");
    if (!Parser::match(TYPES::HASH)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     "JOIN", Reactor::getInstance().getServerIp());
        throw std::exception();
    }
    _channels.push_back(Parser::advance().lexeme());
    while (!Parser::isAtEnd() && Parser::match(TYPES::COMMA)) {
        // Parser::consume(TYPES::HASH, "channel must begin with #.");
        if (!Parser::match(TYPES::HASH)) {
            Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                         "JOIN", Reactor::getInstance().getServerIp());
            throw std::exception();
        }
        if (Parser::isAtEnd()) {
            Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                         "JOIN", Reactor::getInstance().getServerIp());
            throw std::exception();
        }
        _channels.push_back(Parser::advance().lexeme());
    }
}

bool JOIN::_keyIsCorrect(Channel& channel, const size_t& index) {
    return (index < _keys.size()) ? channel.getPassword() == _keys[index]
                                  : false;
}

void JOIN::_setKeys() {
    if (Parser::isAtEnd())
        return;
    // Parser::consume(TYPES::SPACE, "missing space.");
    if (Parser::match(TYPES::SPACE)) {
        Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     "JOIN", Reactor::getInstance().getServerIp());
        throw std::exception();
    }
    _keys.push_back(Parser::advance().lexeme());
    while (!Parser::isAtEnd()) {
        // Parser::consume(TYPES::COMMA, "keys must be separated by ,.");
        if (Parser::match(TYPES::COMMA)) {
            Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                         "JOIN", Reactor::getInstance().getServerIp());
            throw std::exception();
        }
        if (Parser::isAtEnd()) {
            Reply::error(_sender->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                         "JOIN", Reactor::getInstance().getServerIp());
            throw std::exception();
        }
        _keys.push_back(Parser::advance().lexeme());
    }
}

bool JOIN::_channelIsInviteOnly(Channel& channel) {
    return channel.modeIsSet(CHANNEL_MODE::SET_INVITE_ONLY);
}
