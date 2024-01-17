#pragma once

#ifndef __TCHANNELS_HPP__
#define __TCHANNELS_HPP__

#include "Channel.hpp"

class TChannels {
  public:
    static void         create(const std::string& name, Client* client,
                               MEMBER_PERMISSION permission);
    static const size_t size();
    static void         remove(const std::string& name);
    static Channel&     channel(const std::string& name);
    static void         removeMember(const std::string& channel_name,
                                     const std::string& nickname);

  private:
    TChannels();

    static std::map<std::string, Channel> _channels;
};

#endif
