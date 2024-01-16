#include "Message.hpp"
#include "../commands/JOIN.hpp"
#include "../commands/NICK.hpp"
#include "../commands/PASS.hpp"
#include "../commands/USER.hpp"

std::string Message::_password;

TYPES::TokenType Message::_commandTypes[] = {
    TYPES::PASS, TYPES::NICK,   TYPES::USER,  TYPES::JOIN,
    TYPES::KICK, TYPES::INVITE, TYPES::TOPIC, TYPES::MODE};

std::string Message::_commandsStr[] = {"PASS", "NICK",   "USER",  "JOIN",
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
    switch (_whichCommand()) {
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

TYPES::TokenType Message::_whichCommand() {
    for (int i = 0; i < CMDS_NBR; ++i) {
        if (_cmd == _commandsStr[i])
            return _commandTypes[i];
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
    if (!_isCommand()) {
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
    _skipSpaces();
    if (Parser::match(TYPES::SEMICOLON)) {
        _parameters.push_back(Parser::previous().lexeme());
        _trailing();
    } else if (_nospcrlfcl()) {
        _middle();
        _params();
    }
}

void Message::_trailing() {
    Parser::skipSpaces();
    if (!_nospcrlfcl())
        return;
    std::string param;
    do {
        param.append(Parser::advance().lexeme());
        if (Parser::skipSpaces() && !Parser::check(TYPES::CRLF) &&
            !Parser::isAtEnd())
            param.append(Parser::previous().lexeme());
    } while (_nospcrlfcl());
    Parser::skipSpaces();
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

void Message::_skipSpaces() {
    while (Parser::check(TYPES::SPACE))
        Parser::advance();
}

bool Message::_isCommand() {
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

const std::string& Message::getCommand() const {
    return _cmd;
}

const std::vector<std::string>& Message::getParameters() const {
    return _parameters;
}

const std::string& Message::getPassword() {
    return _password;
}
