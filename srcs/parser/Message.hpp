#pragma once

#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include "../commands/Command.hpp"
#include "ErrorReplies.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

class Message {
  public:
    Message();
    Message(const std::string& message);
    ~Message();

    void parse(const std::string& message);
    void execute(const std::string& password);

    const std::string&              get_command() const;
    const std::vector<std::string>& get_parameters() const;

    static const std::string& get_password();

  private:
    Message(const Message&);
    Message& operator=(const Message&);

    enum { MAX_MSG_LEN = 512, MAX_PARAMS = 15, CMDS_NBR = 8 };

    static TYPES::TokenType _command_types[CMDS_NBR];
    static std::string      _commands_str[CMDS_NBR];

    std::string              _cmd;
    std::vector<std::string> _parameters;
    static std::string       _password;
    Command*                 _cmdfunc;

    TYPES::TokenType _which_command();

    void _command();
    void _params();
    bool _nospcrlfcl();
    void _trailing();
    void _middle();

    void _skip_spaces();
    bool _is_command();
};

#endif
