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

    void _setChannels();
    void _setKeys();
};

#endif
