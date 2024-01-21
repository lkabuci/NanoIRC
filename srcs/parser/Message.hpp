#pragma once

#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include "../client/Client.hpp"
#include "../commands/Command.hpp"
#include "../reply/Reply.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

class Message {
  public:
    Message();
    Message(const std::string& message);
    ~Message();

    void parse(Client* client);
    void execute(const std::string& password);

    const std::string&              getCommand() const;
    const std::vector<std::string>& getParameters() const;

    static const std::string& getPassword();

  private:
    Message(const Message&);
    Message& operator=(const Message&);

    enum { MAX_MSG_LEN = 512, MAX_PARAMS = 15, CMDS_NBR = 11 };

    static TYPES::TokenType _commandTypes[CMDS_NBR];
    static std::string      _commandsStr[CMDS_NBR];

    Client*                  _client;
    std::string              _message;
    std::string              _cmd;
    std::vector<std::string> _parameters;
    static std::string       _password;
    Command*                 _cmdfunc;
    static uint8_t           _nbrOfParams;

    TYPES::TokenType _whichCommand();

    std::string _getMessage(const std::string& message);
    void        _command();
    void        _params();
    bool        _nospcrlfcl();
    void        _trailing();
    void        _middle();
    void        _crlf();

    void _skipSpaces();
    bool _isCommand();
};

#endif
