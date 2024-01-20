#pragma once

#ifndef __REPLY_HPP__
#define __REPLY_HPP__

#include "../../include/IRC.h"

namespace SUCCESS_CODES {
enum CODES {

};
}

namespace ERROR_CODES {
enum CODES {
    ERR_ALREADYREGISTRED = 462,
    ERR_NEEDMOREPARAMS = 461,
    ERR_NONICKNAMEGIVEN = 431,
    ERR_NICKNAMEINUSE = 433,
    ERR_NOSUCHCHANNEL = 403,
    ERR_INVITEONLYCHAN = 473,
    ERR_BADCHANNELKEY = 475,
    ERR_NORECIPIENT = 411,
    ERR_NOTEXTTOSEND = 412,
    ERR_TOOMANYTARGETS = 407,
    ERR_NOSUCHNICK = 401,
    ERR_NICKCOLLISION = 436,
    ERR_UNKNOWNCOMMAND = 401,
    ERR_NOTREGISTERED = 451,
};
}

class Reply {
  public:
    static void success(int fd, SUCCESS_CODES::CODES code,
                        const std::string& identifier,
                        const std::string& servername,
                        const std::string& message);
    static void error(int fd, ERROR_CODES::CODES code,
                      const std::string& identifier,
                      const std::string& servername);
    static void sendn(int fd, const std::string& message);

  private:
    Reply();
    static std::map<SUCCESS_CODES::CODES, std::string> _fillSuccessMap();
    static std::map<ERROR_CODES::CODES, std::string>   _fillErrorMap();

    static std::map<SUCCESS_CODES::CODES, std::string> _successReply;
    static std::map<ERROR_CODES::CODES, std::string>   _errorReply;
};

#endif