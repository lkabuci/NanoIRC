#include "USER.hpp"

USER::USER() {}

USER::USER(const USER& user)
    : _username(user._username), _realname(user._realname) {}

USER::~USER() {}

USER& USER::operator=(const USER& user) {
    if (this == &user)
        return *this;
    _username = user._username;
    _realname = user._realname;
    return *this;
}

void USER::execute(const std::string& first, const std::string& parameters) {
    Parser::init(parameters);
    parse_username();
    parse_realname();
    std::cout << "username: " << _username << '\n';
    std::cout << "realname: " << _realname << '\n';
}

void USER::parse_username() {
    if (Parser::is_at_end()) {
        ErrorReplies::reply(2, "localhost", ERROR_CODES::ERR_NEEDMOREPARAMS,
                            "chi wa7d");
        throw std::exception();
    }
    _username = Parser::advance().lexeme();
}

void USER::parse_realname() {
    Parser::consume(TYPES::SEMICOLON, ERROR_CODES::ERR_UNKNOWNCOMMAND);
    if (Parser::is_at_end()) {
        ErrorReplies::reply(2, "localhost", ERROR_CODES::ERR_NEEDMOREPARAMS,
                            "chi wa7d");
        throw std::exception();
    }
    while (!Parser::is_at_end()) {
        if (Parser::check(TYPES::SPACE)) {
            Parser::skip_spaces();
            _realname.append(" ");
        } else
            _realname.append(Parser::advance().lexeme());
    }
}

const std::string& USER::get() const {
    return _username;
}

const std::string& USER::username() const {
    return _username;
}

const std::string& USER::realname() const {
    return _realname;
}
