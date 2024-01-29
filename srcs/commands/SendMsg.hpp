#pragma once

#ifndef __SEND_MSG_HPP__
#define __SEND_MSG_HPP__

#include "Command.hpp"

class SendMsg {
  public:
    static void sendMessage(Client*                         client,
                            const std::vector<std::string>& parameters,
                            const std::string&              command);

  private:
    SendMsg();

    static std::string              _cmd;
    static std::string              _textToSend;
    static std::vector<std::string> _users;
    static std::vector<std::string> _channels;
    static Client*                  _sender;

    static void _parseReceivers();
    static void _addUser();
    static void _addChannel();
    static void _parseText();
    static void _sendText();
    static void _sendToUser(const std::string& name);
    static void _sendToChannel(const std::string& name);
    static void _clear();
    static void _errNotRegistered();
    static void _errNoRecipent();
    static void _errNoTextToSend();
    static void _errNoSuchNick(const std::string& name);
    static void _errNoSuchChannel(const std::string& name);
};

#endif
