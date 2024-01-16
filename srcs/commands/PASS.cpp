#include "PASS.hpp"

PASS::PASS() {}

PASS::PASS(const PASS& p) : _password(p._password) {}

PASS::~PASS() {}

PASS& PASS::operator=(const PASS& p) {
    if (this == &p)
        return *this;
    _password = p._password;
    return *this;
}

void PASS::execute(const std::string& first, const std::string& password) {
    Parser::init(password);
    if (Parser::is_at_end()) {
        ErrorReplies::reply(2, "me", ERROR_CODES::ERR_PASSWDMISMATCH,
                            "chiwa7d");
        throw std::exception();
    }
    if (Parser::advance().lexeme() != first || !Parser::is_at_end()) {
        ErrorReplies::reply(2, "me", ERROR_CODES::ERR_PASSWDMISMATCH,
                            "chiwa7d");
        throw std::exception();
    }
    _password = Parser::previous().lexeme();
}

const std::string& PASS::get() const {
    return _password;
}
