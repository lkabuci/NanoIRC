#include "Message.hpp"
#include "../commands/JOIN.hpp"
#include "../commands/NICK.hpp"
#include "../commands/PASS.hpp"
#include "../commands/PRIVMSG.hpp"
#include "../commands/USER.hpp"

std::string Message::_password;

TYPES::TokenType Message::_commandTypes[] = {
    TYPES::PASS,   TYPES::NICK,  TYPES::USER, TYPES::JOIN,   TYPES::KICK,
    TYPES::INVITE, TYPES::TOPIC, TYPES::MODE, TYPES::PRIVMSG};

std::string Message::_commandsStr[] = {"PASS",  "NICK", "USER",
                                       "JOIN",  "KICK", "INVITE",
                                       "TOPIC", "MODE", "PRIVMSG"};

Message::Message() : _client(NULL), _cmdfunc(NULL) {}

Message::Message(const std::string& message) : _client(NULL), _cmdfunc(NULL) {
    if (message.length() >= MAX_MSG_LEN)
        throw std::runtime_error("message too large.");
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
    case TYPES::PRIVMSG:
        _cmdfunc = new PRIVMSG();
        break;
    default:
        break;
    }
    if (_cmdfunc)
        _cmdfunc->execute(_client, _parameters);
    _client->setIsDoneReading(false);
    _client->clearMessage();
}

TYPES::TokenType Message::_whichCommand() {
    for (int i = 0; i < CMDS_NBR; ++i) {
        if (_cmd == _commandsStr[i])
            return _commandTypes[i];
    }
    return TYPES::END;
}

void Message::parse(Client* client) {
    _client = client;
    _message = _client->getMessage();
    if (_message.empty())
        return;
    std::string msg(_message);
    size_t      crfl_pos = _message.rfind("\r\n");
    if (crfl_pos == std::string::npos) {
        size_t lf_pos = msg.rfind("\n");
        msg.insert(lf_pos, "\r");
    }
    Parser::init(msg);
    _command();
    _params();
    Parser::consume(TYPES::CRLF, "messing CRLF at end.");
}

void Message::_command() {
    if (!_isCommand()) {
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
    case TYPES::PRIVMSG:
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
