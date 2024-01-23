#include "Message.hpp"
#include "../commands/JOIN.hpp"
#include "../commands/NICK.hpp"
#include "../commands/NOTICE.hpp"
#include "../commands/PASS.hpp"
#include "../commands/PRIVMSG.hpp"
#include "../commands/QUIT.hpp"
#include "../commands/USER.hpp"

std::string Message::_password;
uint8_t     Message::_nbrOfParams;

TYPES::TokenType Message::_commandTypes[] = {
    TYPES::PASS,    TYPES::NICK,   TYPES::USER,  TYPES::JOIN,
    TYPES::KICK,    TYPES::INVITE, TYPES::TOPIC, TYPES::MODE,
    TYPES::PRIVMSG, TYPES::NOTICE, TYPES::QUIT,  TYPES::PONG};

std::string Message::_commandsStr[] = {"PASS",    "NICK",   "USER",  "JOIN",
                                       "KICK",    "INVITE", "TOPIC", "MODE",
                                       "PRIVMSG", "NOTICE", "QUIT",  "PONG"};

Message::Message() : _client(NULL), _cmdfunc(NULL) {}

Message::Message(const std::string& message) : _client(NULL), _cmdfunc(NULL) {}

Message::~Message() {
    _reset();
}

void Message::run(Client* client) {
    _client = client;
    _message = _client->getMessage();
    if (_message.empty())
        return;
    std::string temp(_message);
    std::string msg;
    try {
        do {
            msg = _getMessage(temp);
            parse(msg);
            execute("");
            _reset();
            temp = temp.substr(temp.length());
        } while (!temp.empty());
    } catch (const std::exception& e) {
    }
}

void Message::parse(const std::string& message) {
    Parser::init(message);
    _command();
    _params();
    _crlf();
}

void Message::_crlf() {
    if (!Parser::match(TYPES::CRLF)) {
        Reply::error(_client->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     _client->getUserInfo().getNickname(), _cmd);
        throw std::exception();
    }
}

std::string Message::_getMessage(std::string& msg) {
    size_t crlf_pos;

    crlf_pos = msg.find(CR_LF);
    if (crlf_pos == std::string::npos) {
        size_t lf_pos = msg.find("\n");
        msg.insert(lf_pos, "\r");
        return msg.substr(0, lf_pos + 2);
    }
    return msg.substr(0, crlf_pos + 2);
}

void Message::_command() {
    if (!_isCommand()) {
        Reply::error(_client->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     _client->getUserInfo().getNickname(),
                     Parser::peek().lexeme());
        throw std::exception();
    }
    _cmd = Parser::advance().lexeme();
}

void Message::_params() {
    if (Parser::check(TYPES::CRLF) || _nbrOfParams >= MAX_PARAMS)
        return;
    if (!Parser::skipSpaces()) {
        Reply::error(_client->getSockfd(), ERROR_CODES::ERR_UNKNOWNCOMMAND,
                     _client->getUserInfo().getNickname(), _cmd);
        throw std::exception();
    }
    ++_nbrOfParams;
    if (Parser::match(TYPES::COLON)) {
        _parameters.push_back(Parser::previous().lexeme());
        _trailing();
    } else if (_nospcrlfcl()) {
        _middle();
        _params();
    }
}

void Message::_trailing() {
    if (Parser::isAtEnd() || Parser::check(TYPES::CRLF))
        return;
    std::string param;
    do {
        param.append(Parser::advance().lexeme());
    } while (_nospcrlfcl() || Parser::check(TYPES::SPACE) ||
             Parser::check(TYPES::COLON));
    _parameters.push_back(param);
}

void Message::_middle() {
    std::string param;
    while (_nospcrlfcl() || Parser::check(TYPES::COLON))
        param.append(Parser::advance().lexeme());
    _parameters.push_back(param);
}

bool Message::_nospcrlfcl() {
    switch (Parser::peek().type()) {
    case TYPES::CR:
    case TYPES::LF:
    case TYPES::SPACE:
    case TYPES::COLON:
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
    case TYPES::NOTICE:
    case TYPES::QUIT:
    case TYPES::PONG:
        return true;
    default:
        break;
    }
    return false;
}

const std::string& Message::getPassword() {
    return _password;
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
    case TYPES::NOTICE:
        _cmdfunc = new NOTICE();
        break;
    case TYPES::QUIT:
        _cmdfunc = new QUIT();
        break;
    case TYPES::PONG:
        return;
    default:
        break;
    }
    if (!_cmdfunc)
        return;
    try {
        _cmdfunc->execute(_client, _parameters);
    } catch (...) {
    }
    _client->finish();
}

TYPES::TokenType Message::_whichCommand() {
    for (int i = 0; i < CMDS_NBR; ++i) {
        if (_cmd == _commandsStr[i])
            return _commandTypes[i];
    }
    return TYPES::END;
}

void Message::_reset() {
    _nbrOfParams = 0;
    _parameters.clear();
    delete _cmdfunc;
    _cmdfunc = NULL;
}
