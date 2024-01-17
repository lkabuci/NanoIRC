#pragma once

#include <vector>
#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include "../../include/IRC.h"
#include "../client/Client.hpp"

namespace CHANNEL_MODE {
enum Modes {
    SET_INVITE_ONLY,
    REMOVE_INVITE_ONLY,
    SET_TOPIC,
    REMOVE_TOPIC,
    SET_KEY,
    REMOVE_KEY,
    GIVE_PRIVILEGE,
    TAKE_PRIVILEGE,
    SET_LIMIT,
    REMOVE_LIMIT
};
}

enum MEMBER_PERMISSION { OPERATOR, REGULAR };

class Channel {
  public:
    Channel(const std::string& name);

    void               add(Client* newMember, MEMBER_PERMISSION premission);
    void               setMode(CHANNEL_MODE::Modes mode);
    void               updateMode(CHANNEL_MODE::Modes mode); // temporary
    const size_t       getNumberOfMembers() const;
    void               sendToAll(const std::string& message);
    void               remove(const std::string& username);
    void               remove(Client* client);
    void               clear();
    bool               empty() const;
    const std::string& name() const;
    bool               modeIsSet(CHANNEL_MODE::Modes mode);
    bool               flagIsSet(Client* client, MEMBER_PERMISSION flag);
    Client*            getClient(const std::string& username);

  private:
    std::string                          _name;
    std::map<Client*, MEMBER_PERMISSION> _members;
    CHANNEL_MODE::Modes                  _mode;
};

CHANNEL_MODE::Modes operator|(CHANNEL_MODE::Modes a, CHANNEL_MODE::Modes b);
CHANNEL_MODE::Modes operator&(CHANNEL_MODE::Modes a, CHANNEL_MODE::Modes b);

#endif
