#pragma once

#ifndef __PASS_HPP__
#define __PASS_HPP__

#include "Command.hpp"

class PASS : public Command {
  public:
    PASS();
    PASS(const PASS& p);
    virtual ~PASS();

    PASS& operator=(const PASS& p);

    virtual void       execute(const std::vector<std::string>& parameters);
    const std::string& getPassword() const;

  private:
    std::string _password;
};

#endif
