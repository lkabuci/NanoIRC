#pragma once

#ifndef __JOIN_HPP__
#define __JOIN_HPP__

#include "Command.hpp"

class JOIN : public Command {
  public:
    JOIN();
    JOIN(const JOIN& join);
    virtual ~JOIN();

    JOIN& operator=(const JOIN& join);

    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);

  private:
    std::vector<std::string> _channels;
    std::vector<std::string> _keys;
    Client*                  _sender;

    void _setChannels();
    void _addChannel();
    void _setKeys();
    void _addKey();
    void _joinUser();
    void _createChannel(const size_t& index);
    void _addToChannel(Channel& channel, const size_t& index);
    bool _channelIsInviteOnly(Channel& channel);
    bool _keyIsCorrect(Channel& channel, const size_t& index);
    bool _channelHasKey(Channel& channel);
    void _addClientToChannel(Channel& channel, MEMBER_PERMISSION::Flags flag);
    bool _userIsRegistered();
    void _sendSuccessReply(const std::string& name);
    void _leaveAllChannels();
};

#endif
