#pragma once

#ifndef __PRIVMSG_HPP__
#define __PRIVMSG_HPP__

#include "Command.hpp"

class PRIVMSG : public Command {
  public:
    PRIVMSG();
    PRIVMSG(const PRIVMSG& privmsg);
    virtual ~PRIVMSG();

    PRIVMSG& operator=(const PRIVMSG& privmsg);

    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);

  private:
    std::vector<std::string> _users;
    std::vector<std::string> _channels;
    std::string              _textToSent;
    Client*                  _sender;

    void _parseReceivers();
    void _parseText();
    void _sentText();
    void _addChannel();
    void _addUser();
    bool _userAlreadyExists(const std::string& name);
    bool _channelAlreadyExists(const std::string& name);
    bool _userBelongToChannel(const std::string& user);
    void _sendToUser(const std::string& name);
};

#endif