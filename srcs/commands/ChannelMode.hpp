#pragma once

#ifndef __CHANNEL_MODE_HPP__
#define __CHANNEL_MODE_HPP__

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

class ChannelMode {
  public:
    ChannelMode();
    ChannelMode(const CHANNEL_MODES& mode);
    ChannelMode(const ChannelMode& channel);
    ~ChannelMode();

    ChannelMode& operator=(const ChannelMode& channel);

    const CHANNEL_MODES& getMode() const;
    void                 setChannelMode(CHANNEL_MODES mode);
    void                 updateChannelMode(CHANNEL_MODES mode);

  private:
    CHANNEL_MODES _mode;
};

CHANNEL_MODES operator|(CHANNEL_MODES a, CHANNEL_MODES b);
bool          operator&(CHANNEL_MODES a, CHANNEL_MODES b);

#endif
