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
    ERR_NOTONCHANNEL = 442,

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
    ERR_CHANOPRIVSNEEDED = 482,
    ERR_USERONCHANNEL = 443,
    ERR_BADCHANMASK = 476,
    ERR_UNKNOWNMODE = 472,
    ERR_USERSDONTMATCH = 502,
    ERR_KEYSET = 467,
    ERR_UMODEUNKNOWNFLAG = 501,
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

    static void errUnknownCommand(Client* client, const std::string& cmd);
    static void errNotEnoughParams(Client* client, const std::string& cmd);
    static void errNoSuchChannel(Client* client, const std::string& name);
    static void errInviteOnlyChan(Client* client, const std::string& name);
    static void errChannelIsFull(Client* client, const std::string& name);
    static void errBadChannelKey(Client* client, const std::string& name);
    static void channelReply(Client* client, const std::string& name);
    static void welcome(Client* client);

    static void errPasswdMismatch(Client* client);
    static void errAlreadyRegistred(Client* client);

    static void errNoSuchNick(Client* client, const std::string& name);
    static void errNotRegistered(Client* client);
    static void errNoTextToSend(Client* client);
    static void errNoRecipient(Client* client, const std::string& cmd);

    static void errErroneousNickname(Client* client, const std::string& name);
    // ERR_CANNOTSENDTOCHAN
    static void errCannotSendToChan(Client* client, const std::string& channel,
                                    const std::string& nickname);

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