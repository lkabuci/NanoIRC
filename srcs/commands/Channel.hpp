#pragma once

#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include "../../include/IRC.h"

enum CHANNEL_MODES {
    NO_FLAG,
    SET_INVITE_ONLY,
    REMOVE_INVITE_ONLY,
    SET_TOPIC,
    REMOVE_TOPIC,
    SET_KEY,
    REMOVE_KEY,
    GIVE_OPT_PRIVILEGE,
    TAKE_OPT_PRIVILEGE,
    SET_USER_LIMIT,
    REMOVE_USER_LIMIT
};

enum USER_MODES { REGULAR = 0x00, ADMIN = 0x02, OWNER = 0x04 };

class Channel {
  public:
    Channel();
    Channel(const CHANNEL_MODES& mode /*, const Client& client*/);
    Channel(const Channel& channel);
    ~Channel();

    Channel& operator=(const Channel& channel);

    const CHANNEL_MODES& getMode() const;
    // const Client&        get_client(USER_MODES mode) const;

    void setChannelMode(CHANNEL_MODES mode);
    void updateChannelMode(CHANNEL_MODES mode);

    // void add_client(const Client* client, USER_MODES mode);

  private:
    CHANNEL_MODES _mode;
    // std::map<Client*, USER_MODES>   _clients;
};

CHANNEL_MODES operator|(CHANNEL_MODES a, CHANNEL_MODES b);
bool          operator&(CHANNEL_MODES a, CHANNEL_MODES b);

#endif
