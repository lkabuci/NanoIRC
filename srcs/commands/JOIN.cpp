#include "JOIN.hpp"

JOIN::JOIN() {}

JOIN::JOIN(const JOIN& join) : _channels(join._channels), _keys(join._keys) {}

JOIN::~JOIN() {}

JOIN& JOIN::operator=(const JOIN& join) {
    if (this == &join)
        return *this;
    _channels = join._channels;
    _keys = join._keys;
    return *this;
}

void JOIN::execute(Client* client, const std::vector<std::string>& parameters) {
    if (parameters.empty())
        throw std::runtime_error("JOIN <channel>{,<channel>} [<key>{,<key>}]");
    if (!client->getUserInfo().isRegistered())
        throw std::runtime_error("client is not registered.");

    Parser::init(Utils::join(parameters));
    _setChannels();
    _setKeys();
    _joinUser(client);
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
    if (channel.modeIsSet(CHANNEL_MODE::SET_INVITE_ONLY) &&
        !channel.isInvited(client))
        throw std::runtime_error(
            "472 ERR_INVITEONLYCHAN::Cannot join channel (+i).");
    if (index < _keys.size()) {
        if (channel.getPassword() != _keys[index])
            throw std::runtime_error(
                "475 ERR_BADCHANNELKEY:Cannot join channel (+k)");
    }
    channel.add(client, MEMBER_PERMISSION::REGULAR);
    TChannels::add(_channels[index], channel);
    if (channel.isInvited(client))
        channel.eraseFromInviteeslist(client);
}

void JOIN::_setChannels() {
    Parser::consume(TYPES::HASH, "channel must begin with #.");
    _channels.push_back(Parser::advance().lexeme());
    while (!Parser::isAtEnd() && Parser::match(TYPES::COMMA)) {
        Parser::consume(TYPES::HASH, "channel must begin with #.");
        if (Parser::isAtEnd())
            throw std::runtime_error("missing channel.");
        _channels.push_back(Parser::advance().lexeme());
    }
}

void JOIN::_setKeys() {
    if (Parser::isAtEnd())
        return;
    Parser::consume(TYPES::SPACE, "missing space.");
    _keys.push_back(Parser::advance().lexeme());
    while (!Parser::isAtEnd()) {
        Parser::consume(TYPES::COMMA, "keys must be separated by ,.");
        if (Parser::isAtEnd())
            throw std::runtime_error("missing key.");
        _keys.push_back(Parser::advance().lexeme());
    }
}
