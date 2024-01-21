#pragma once

#ifndef __REPLY_HPP__
#define __REPLY_HPP__

#include "../../include/IRC.h"
#include "../server/Reactor.hpp"

namespace SUCCESS_CODES {
enum CODES {
    RPL_WELCOME = 1,
    RPL_TOPIC = 332,

    //:<server> 324 <nickname> <channel> <mode> <mode-parameters>
    //<#ch1> nick1 has joined (~nick1@freenode-obu.d75.6g0qj4.IP)
    RPL_CHANNELMODEIS = 324,
};
}

namespace ERROR_CODES {
enum CODES {
    //:<server> 462 <nickname> :You may not reregisted
    ERR_ALREADYREGISTRED = 462,

    //:<server> 461 <nickname> <command> :Not enough parameters
    ERR_NEEDMOREPARAMS = 461,
    ERR_NONICKNAMEGIVEN = 431,

    //:<server> 432 <nickname> :Erroneous nickname
    ERR_ERRONEUSNICKNAME = 432,

    //:<server> 433 <nickname> :Nickname is already in use
    ERR_NICKNAMEINUSE = 433,

    //:<server> 403 <nickname> <channel> :No such channel
    ERR_NOSUCHCHANNEL = 403,

    //:<server> 473 <nickname> <channel> :Cannot join channel (+i)
    ERR_INVITEONLYCHAN = 473,

    //:<server> 475 <nickname> <channel> :Cannot join channel (+k)
    ERR_BADCHANNELKEY = 475,

    //:<server> 411 <nickname> :No recipient given (PRIVMSG/NOTICE)
    ERR_NORECIPIENT = 411,

    //:<server> 412 <nickname> :No text to send
    ERR_NOTEXTTOSEND = 412,

    //:<server> 407 <nickname> <command> :Too many targets
    ERR_TOOMANYTARGETS = 407,

    //:<server> 401 <nickname> <target> :No such nickname
    ERR_NOSUCHNICK = 401,

    ERR_NICKCOLLISION = 436,

    //:<server> 421 <nickname> <command> :Unknown command
    ERR_UNKNOWNCOMMAND = 401,

    // :<server> 451 <nickname> :You have not registered
    ERR_NOTREGISTERED = 451,
};
}

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

    static void rpl_channelModeIs(int fd, const std::string& nickname,
                                  const std::string& channel,
                                  const std::string& mode);

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