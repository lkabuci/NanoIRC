#pragma once

#ifndef __JOIN_HPP__
#define __JOIN_HPP__

#include "Command.hpp"

class JOIN : public Command {
  public:
    explicit JOIN();
    virtual ~JOIN();

    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);

  private:
    JOIN(const JOIN& join);
    JOIN& operator=(const JOIN& join);

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
    void _leaveAllChannels();
    bool _validChannel(const std::string& channel);

    void _channelReply(const std::string& channel);
    void _tellMembers(Channel& channel);
    void _errNotEnoughParams(Client* client);
    void _errNoSuchChannel(const std::string& name);
    void _errChannelIsFull(const std::string& name);
    void _errInviteOnlyChan(const std::string& name);
};

#endif
