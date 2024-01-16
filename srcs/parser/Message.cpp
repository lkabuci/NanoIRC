#include "Message.hpp"
#include "../commands/JOIN.hpp"
#include "../commands/NICK.hpp"
#include "../commands/PASS.hpp"
#include "../commands/USER.hpp"

std::string Message::_password;

TYPES::TokenType Message::_command_types[] = {
    TYPES::PASS, TYPES::NICK,   TYPES::USER,  TYPES::JOIN,
    TYPES::KICK, TYPES::INVITE, TYPES::TOPIC, TYPES::MODE};

std::string Message::_commands_str[] = {"PASS", "NICK",   "USER",  "JOIN",
                                        "KICK", "INVITE", "TOPIC", "MODE"};

Message::Message() : _cmdfunc(NULL) {}

Message::Message(const std::string& message) : _cmdfunc(NULL) {
    if (message.length() >= MAX_MSG_LEN)
        throw std::runtime_error("message too large.");
    parse(message);
}

Message::~Message() {
    delete _cmdfunc;
}

void Message::execute(const std::string& password) {
    _password = password;
    switch (_which_command()) {
    case TYPES::PASS:
        _cmdfunc = new PASS();
        break;
    case TYPES::NICK:
        _cmdfunc = new NICK();
        break;
    case TYPES::USER:
        _cmdfunc = new USER();
        break;
    case TYPES::JOIN:
        _cmdfunc = new JOIN();
        break;
    default:
        break;
    }
    if (_cmdfunc)
        _cmdfunc->execute(_parameters);
}

TYPES::TokenType Message::_which_command() {
    for (int i = 0; i < CMDS_NBR; ++i) {
        if (_cmd == _commands_str[i])
            return _command_types[i];
    }
    return TYPES::END;
}

void Message::parse(const std::string& message) {
    if (message.empty())
        return;
    Parser::init(message);
    _command();
    _params();
    Parser::consume(TYPES::CRLF, "messing CRLF at end.");
}

void Message::_command() {
    if (!_is_command()) {
        ErrorReplies::reply(2, "localhost", ERROR_CODES::ERR_UNKNOWNCOMMAND,
                            "c");
        throw std::exception();
    }
    _cmd = Parser::advance().lexeme();
}

void Message::_params() {
    if (Parser::check(TYPES::CRLF))
        return;
    Parser::consume(TYPES::SPACE, "missing space.");
    _skip_spaces();
    if (Parser::match(TYPES::SEMICOLON)) {
        _parameters.push_back(Parser::previous().lexeme());
        _trailing();
    } else if (_nospcrlfcl()) {
        _middle();
        _params();
    }
}

void Message::_trailing() {
    Parser::skip_spaces();
    if (!_nospcrlfcl())
        return;
    std::string param;
    do {
        param.append(Parser::advance().lexeme());
        if (Parser::skip_spaces() && !Parser::check(TYPES::CRLF) &&
            !Parser::is_at_end())
            param.append(Parser::previous().lexeme());
    } while (_nospcrlfcl());
    Parser::skip_spaces();
    _parameters.push_back(param);
}

void Message::_middle() {
    std::string param;
    while (_nospcrlfcl())
        param.append(Parser::advance().lexeme());
    _parameters.push_back(param);
}

bool Message::_nospcrlfcl() {
    switch (Parser::peek().type()) {
    case TYPES::CR:
    case TYPES::LF:
    case TYPES::SPACE:
    case TYPES::SEMICOLON:
    case TYPES::CRLF:
        return false;
    default:
        break;
    }
    return true;
}

void Message::_skip_spaces() {
    while (Parser::check(TYPES::SPACE))
        Parser::advance();
}

bool Message::_is_command() {
    switch (Parser::peek().type()) {
    case TYPES::PASS:
    case TYPES::NICK:
    case TYPES::USER:
    case TYPES::JOIN:
    case TYPES::KICK:
    case TYPES::INVITE:
    case TYPES::TOPIC:
    case TYPES::MODE:
        return true;
    default:
        break;
    }
    return false;
}

const std::string& Message::get_command() const {
    return _cmd;
}

const std::vector<std::string>& Message::get_parameters() const {
    return _parameters;
}

const std::string& Message::get_password() {
    return _password;
}
