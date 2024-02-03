#pragma once

#ifndef __ERROR_CODES_HPP__
#define __ERROR_CODES_HPP__

#include "../../include/IRC.h"

namespace ERROR_CODES {
enum Codes {
    ERR_UNKNOWNCOMMAND = 421,
    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTRED = 462,
    ERR_PASSWDMISMATCH = 464,
};
}

class ErrorReplies {
  public:
    static std::string describe(const ERROR_CODES::Codes& code,
                                const std::string&        str);
    static void        reply(int fd, const std::string& sender,
                             const ERROR_CODES::Codes& code,
                             const std::string&        target);

  private:
    ErrorReplies();

    static std::map<ERROR_CODES::Codes, std::string> _values;
    static std::map<ERROR_CODES::Codes, std::string> _init_values_map();
};

#endif