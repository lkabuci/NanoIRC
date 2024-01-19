#pragma once

#ifndef __SEND_MSG_HPP__
#define __SEND_MSG_HPP__

#include "Command.hpp"

class SendMsg {
  public:
    static void sendMessage(Client*                         client,
                            const std::vector<std::string>& parameters);

  private:
    SendMsg();

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
    static bool _userAlreadyExists(const std::string& name);
    static bool _channelAlreadyExists(const std::string& name);
    static bool _userBelongToChannel(const std::string& user);
    static void _clear();
};

#endif
