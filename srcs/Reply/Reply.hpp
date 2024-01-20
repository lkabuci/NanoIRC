#pragma once

#ifndef __REPLY_HPP__
#define __REPLY_HPP__

#include "../../include/IRC.h"
#include "../server/Reactor.hpp"

namespace SUCCESS_CODES {
enum CODES {
    RPL_WELCOME = 1,
    RPL_TOPIC = 332,
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

//:<server> 462 <nickname> :You may not reregister
//:<server> 461 <nickname> <command> :Not enough parameters

class Reply {
  public:
    static void success(int fd, SUCCESS_CODES::CODES code,
                        const std::string& identifier,
                        const std::string& message);
    static void error(int fd, ERROR_CODES::CODES code, const std::string& s1,
                      const std::string& s2);

    //: server.hostname 001 yournickname :Welcome to the IRC network,
    //: yournickname!user@host
    static void rpl_welcome(int fd, const std::string& nickname,
                            const std::string& username);

    //:<server> 332 <nickname> <channel> :<topic>
    static void rpl_topic(int fd, const std::string& nickname,
                          const std::string& channel, const std::string& topic);

  private:
    Reply();
    static std::map<SUCCESS_CODES::CODES, std::string> _fillSuccessMap();
    static std::map<ERROR_CODES::CODES, std::string>   _fillErrorMap();

    static std::map<SUCCESS_CODES::CODES, std::string> _successReply;
    static std::map<ERROR_CODES::CODES, std::string>   _errorReply;

    //:<server> 436 <nickname> :Nickname collision KILL from <user>@<host>
    static void _err_nickCollision(int fd, const std::string& nickname,
                                   const std::string& username);
};

#endif