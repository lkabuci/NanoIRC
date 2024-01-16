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

void JOIN::execute(const std::vector<std::string>& parameters) {
    if (parameters.empty())
        throw std::runtime_error("JOIN <channel>{,<channel>} [<key>{,<key>}]");
    std::string params = Utils::join(parameters);

    Parser::init(params);
    _setChannels();
    _setKeys();
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
