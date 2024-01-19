#pragma once

#ifndef __NOTICE_HPP__
#define __NOTICE_HPP__

#include "Command.hpp"

class NOTICE : public Command {
  public:
    NOTICE();
    virtual ~NOTICE();

    virtual void execute(Client*                         client,
                         const std::vector<std::string>& parameters);

  private:
    NOTICE(const NOTICE&);
    NOTICE& operator=(const NOTICE&);

    void _sendToChannel();
    void _sendToUser(Client* client);
};

#endif
