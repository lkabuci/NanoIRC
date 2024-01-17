#pragma once

#include <vector>
#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include "../../include/IRC.h"
#include "../client/Client.hpp"

namespace CHANNEL_MODE {
enum Modes {
    NO_PASSWORD,
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
    Channel(const std::string& name, const std::string& password = "",
            CHANNEL_MODE::Modes mode = CHANNEL_MODE::NO_PASSWORD);
    Channel();

    void               add(Client* newMember, MEMBER_PERMISSION premission);
    void               setMode(CHANNEL_MODE::Modes mode);
    void               updateMode(CHANNEL_MODE::Modes mode); // temporary
    size_t             getNumberOfMembers() const;
    void               sendToAll(const std::string& message);
    void               remove(const std::string& username);
    void               remove(Client* client);
    void               clear();
    bool               empty() const;
    bool               exist(Client* client);
    const std::string& name() const;
    bool               modeIsSet(CHANNEL_MODE::Modes mode);
    bool               flagIsSet(Client* client, MEMBER_PERMISSION flag);
    Client*            getClient(const std::string& username);
    const std::string& getPassword() const;
    void               setPassword(const std::string& password);
    void               invite(Client* client);
    bool               isInvited(Client* client);

  private:
    std::string                          _name;
    std::string                          _password;
    std::map<Client*, MEMBER_PERMISSION> _members;
    std::vector<Client*>                 _invited;
    CHANNEL_MODE::Modes                  _mode;
};

CHANNEL_MODE::Modes operator|(CHANNEL_MODE::Modes a, CHANNEL_MODE::Modes b);
CHANNEL_MODE::Modes operator&(CHANNEL_MODE::Modes a, CHANNEL_MODE::Modes b);

#endif
