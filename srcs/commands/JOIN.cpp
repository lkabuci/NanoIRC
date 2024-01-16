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
    std::string params = Utils::combine(parameters);

    Parser::init(params);
    set_channels();
    set_keys();
    std::cout << "channels: ";
    for (size_t i = 0; i < _channels.size(); ++i)
        std::cout << _channels[i] << ", ";
    std::cout << '\n';
    std::cout << "keys: ";
    for (size_t i = 0; i < _keys.size(); ++i)
        std::cout << _keys[i] << ", ";
    std::cout << '\n';
}

void JOIN::set_channels() {
    Parser::consume(TYPES::HASH, "channel must begin with #.");
    _channels.push_back(Parser::advance().lexeme());
    while (!Parser::is_at_end() && Parser::match(TYPES::COMMA)) {
        Parser::consume(TYPES::HASH, "channel must begin with #.");
        if (Parser::is_at_end())
            throw std::runtime_error("missing channel.");
        _channels.push_back(Parser::advance().lexeme());
    }
}

void JOIN::set_keys() {
    if (Parser::is_at_end())
        return;
    Parser::consume(TYPES::SPACE, "missing space.");
    _keys.push_back(Parser::advance().lexeme());
    while (!Parser::is_at_end()) {
        Parser::consume(TYPES::COMMA, "keys must be separated by ,.");
        if (Parser::is_at_end())
            throw std::runtime_error("missing key.");
        _keys.push_back(Parser::advance().lexeme());
    }
}
