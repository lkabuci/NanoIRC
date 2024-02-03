#pragma once

#ifndef __USER_HPP__
#define __USER_HPP__

#include "Command.hpp"

class USER : public Command {
  public:
    USER();
    USER(const USER& user);
    virtual ~USER();

    USER& operator=(const USER& user);

    virtual void       execute(const std::vector<std::string>& parameters);
    const std::string& getUsername() const;
    const std::string& getRealname() const;

  private:
    std::string _username;
    std::string _realname;
};

#endif
