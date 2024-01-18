#pragma once

#include <vector>
#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include "../../include/IRC.h"

class Client;

namespace CHANNEL_MODE {
enum Modes {
    NO_MODE = 0,
    SET_INVITE_ONLY = 2,
    SET_TOPIC = 4,
    SET_KEY = 8,
    GIVE_PRIVILEGE = 16,
    SET_LIMIT = 32,
};
}

namespace MEMBER_PERMISSION {
enum Flags { OPERATOR, REGULAR };
}

class Channel {
  public:
    Channel();
    Channel(const std::string& name, const std::string& password = "",
            CHANNEL_MODE::Modes mode = CHANNEL_MODE::NO_MODE);
    Channel(const Channel& channel);

    Channel& operator=(const Channel& channel);

    void   add(Client* newMember, MEMBER_PERMISSION::Flags premission);
    void   setMode(CHANNEL_MODE::Modes mode);
    void   unsetMode(CHANNEL_MODE::Modes mode);
    void   updateMode(CHANNEL_MODE::Modes mode); // temporary
    size_t getNumberOfMembers() const;
    void   sendToAll(const std::string& message);
    void   remove(const std::string& username);
    void   remove(Client* client);
    void   clear();
    bool   empty() const;
    bool   exist(Client* client);
    bool   exist(const std::string& nickname);
    const std::string& name() const;
    bool               modeIsSet(CHANNEL_MODE::Modes mode);
    bool               flagIsSet(Client* client, MEMBER_PERMISSION::Flags flag);
    Client*            getClient(const std::string& username);
    const std::string& getPassword() const;
    void               setPassword(const std::string& password);
    void               invite(Client* client);
    bool               isInvited(Client* client);
    void               eraseFromInvitedlist(Client* client);
    const std::string& getTopic() const;
    void               setTopic(const std::string& topic);

  private:
    std::string                                 _name;
    std::string                                 _password;
    std::map<Client*, MEMBER_PERMISSION::Flags> _members;
    std::vector<Client*>                        _invited;
    CHANNEL_MODE::Modes                         _mode;
    std::string                                 _topic;
};

CHANNEL_MODE::Modes operator|(CHANNEL_MODE::Modes a, CHANNEL_MODE::Modes b);
CHANNEL_MODE::Modes operator&(CHANNEL_MODE::Modes a, CHANNEL_MODE::Modes b);

#endif
