#pragma once

#ifndef __REPLY_HPP__
#define __REPLY_HPP__

#include "../../include/IRC.h"
#include "../server/Reactor.hpp"

class Reply {
  public:
    static void errUnknownCommand(Client* client, const std::string& cmd);
    static void errNotEnoughParams(Client* client, const std::string& cmd);
    static void errNoSuchChannel(Client* client, const std::string& name);
    static void errInviteOnlyChan(Client* client, const std::string& name);
    static void errChannelIsFull(Client* client, const std::string& name);
    static void errBadChannelKey(Client* client, const std::string& name);
    static void channelReply(Client* client, const std::string& name);
    static void welcome(Client* client);

    static void errPasswdMismatch(Client* client);
    static void errAlreadyRegistred(Client* client);

    static void errNoSuchNick(Client* client, const std::string& name);
    static void errNotRegistered(Client* client);
    static void errNoTextToSend(Client* client);
    static void errNoRecipient(Client* client, const std::string& cmd);

    static void errErroneousNickname(Client* client, const std::string& name);
    static void errCannotSendToChan(Client* client, const std::string& channel,
                                    const std::string& nickname);

  private:
    Reply();
};

#endif