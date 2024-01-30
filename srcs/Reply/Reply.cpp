#include "Reply.hpp"
#include "../Utils/Utils.hpp"

std::map<SUCCESS_CODES::CODES, std::string> Reply::_successReply =
    _fillSuccessMap();
std::map<ERROR_CODES::CODES, std::string> Reply::_errorReply = _fillErrorMap();

void Reply::success(int fd, SUCCESS_CODES::CODES code,
                    const std::string& identifier, const std::string& message) {

    // std::string msg = std::string(":") + Reactor::getInstance().getServerIp()
    // +
    //                   " " + Utils::toStr(code) + " " + identifier + " " +
    //                   _successReply[code] + message + CR_LF;

    std::string msg = std::string(":") + "localhost " + Utils::toStr(code) +
                      " " + identifier + " " + _successReply[code] + message +
                      CR_LF;

    send(fd, msg.c_str(), msg.size(), 0);
}

//: stockholm.se.quakenet.org 411 i2 :No recipient given (PRIVMSG)
void Reply::error(int fd, ERROR_CODES::CODES code, const std::string& s1,
                  const std::string& s2) {

    if (code == ERROR_CODES::ERR_NICKCOLLISION) {
        _err_nickCollision(fd, s1, s2);
        return;
    }

    // std::string msg = std::string(":") + Reactor::getInstance().getServerIp()
    // +
    //                   " " + Utils::toStr(code) + " " + s1 + " " + s2 + " " +
    //                   _errorReply[code] + CR_LF;

    std::string msg = std::string(":") + "localhost" + " " +
                      Utils::toStr(code) + " " + s1 + " " + s2 + " " +
                      _errorReply[code] + CR_LF;

    send(fd, msg.c_str(), msg.length(), 0);
}

void Reply::_err_nickCollision(int fd, const std::string& nickname,
                               const std::string& username) {

    std::string msg =
        std::string(":") + Reactor::getInstance().getServerIp() + " 436 " +
        nickname + _errorReply[ERROR_CODES::ERR_NICKCOLLISION] + " " +
        username + "@" + Reactor::getInstance().getServerIp() + CR_LF;

    send(fd, msg.c_str(), msg.length(), 0);
}

void Reply::rpl_welcome(int fd, const std::string& nickname,
                        const std::string& username) {
    // std::string msg = std::string(":") + Reactor::getInstance().getServerIp()
    // +
    //                   " 001 " + nickname + " :Welcome to the IRC network, " +
    //                   nickname + "!" + username + "@" +
    //                   Reactor::getInstance().getServerIp() + CR_LF;

    std::string msg = std::string(":") + "localhost 001 " + nickname +
                      " :Welcome to the IRC network, " + nickname + "!" +
                      username + "@localhost\r\n";

    send(fd, msg.c_str(), msg.length(), 0);
}

void Reply::rpl_topic(int fd, const std::string& nickname,
                      const std::string& channel, const std::string& topic) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      " 332 " + nickname + " " + channel + " :" + topic + CR_LF;

    send(fd, msg.c_str(), msg.length(), 0);
}

void Reply::rpl_channelModeIs(int fd, const std::string& nickname,
                              const std::string& channel,
                              const std::string& mode) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      " 324 " + " " + nickname + " " + channel + " " + mode +
                      CR_LF;

    send(fd, msg.c_str(), msg.length(), 0);
}

std::map<ERROR_CODES::CODES, std::string> Reply::_fillErrorMap() {
    std::map<ERROR_CODES::CODES, std::string> ret;

    ret[ERROR_CODES::ERR_ALREADYREGISTRED] = ":You may not register";
    ret[ERROR_CODES::ERR_NEEDMOREPARAMS] = ":Not enough parameters";
    ret[ERROR_CODES::ERR_NONICKNAMEGIVEN] = ":No nickname given";
    ret[ERROR_CODES::ERR_ERRONEUSNICKNAME] = ":Erroneus nickname";
    ret[ERROR_CODES::ERR_NICKNAMEINUSE] = ":Nickname is already in use";
    ret[ERROR_CODES::ERR_NOSUCHCHANNEL] = ":No such channel";
    ret[ERROR_CODES::ERR_INVITEONLYCHAN] = ":Cannot join channel (+i)";
    ret[ERROR_CODES::ERR_BADCHANNELKEY] = ":Cannot join channel (+k)";
    ret[ERROR_CODES::ERR_NORECIPIENT] = ":No recipient given (PRIVMSG/NOTICE)";
    ret[ERROR_CODES::ERR_NOTEXTTOSEND] = ":No text to send";
    ret[ERROR_CODES::ERR_TOOMANYTARGETS] =
        ":Duplicate recipients. No message delivered";
    ret[ERROR_CODES::ERR_NOSUCHNICK] = ":No such nick";
    ret[ERROR_CODES::ERR_NICKCOLLISION] = ":Nickname collision KILL from";
    ret[ERROR_CODES::ERR_UNKNOWNCOMMAND] = ":Unknown command";
    ret[ERROR_CODES::ERR_NOTREGISTERED] = ":You have not registered";
    ret[ERROR_CODES::ERR_NOTONCHANNEL] = ":You're not on that channel";
    ret[ERROR_CODES::ERR_CHANOPRIVSNEEDED] = ":You're not channel operator";
    ret[ERROR_CODES::ERR_USERONCHANNEL] = ":is already on channel";
    return (ret);
}

std::map<SUCCESS_CODES::CODES, std::string> Reply::_fillSuccessMap() {
    std::map<SUCCESS_CODES::CODES, std::string> ret;

    ret[SUCCESS_CODES::RPL_WELCOME] = "Welcome to the Internet Relay Network";
    ret[SUCCESS_CODES::RPL_TOPIC] = "";
    return ret;
}

void sendn(int fd, const std::string& message) {
    int         numWritten;
    int         totWritten;
    int         len = static_cast<int>(message.length());
    const char* buffer = message.c_str();

    for (totWritten = 0; totWritten < len;) {
        numWritten = send(fd, buffer, len - totWritten, 0);

        if (numWritten <= 0) {
            if (numWritten == -1) {
                std::perror("send");
                throw std::exception();
            }
            return;
        }
        totWritten += numWritten;
        buffer += numWritten;
    }
}

void Reply::welcome(Client* client) {
    std::string server_name = std::string(":") + Reactor::getServerName();
    std::string rpl_welcome =
        server_name + " 001 " + client->getUserInfo().getNickname() +
        " :Welcome to the Internet Relay Network " +
        client->getUserInfo().getNickname() + "!" +
        client->getUserInfo().getUsername() + "@" + client->getIp() + CR_LF;
    std::string rpl_yourhost =
        server_name + " 002 " + client->getUserInfo().getNickname() +
        " :Your host is " + server_name + ", running version v1\r\n";
    std::string rpl_created =
        server_name + " 003 " + client->getUserInfo().getNickname() +
        " :This server was created Tue 16 Jan 2024 at 14:47\r\n";
    std::string rpl_myinfo = server_name + " 004 " +
                             client->getUserInfo().getNickname() + server_name +
                             " i1 itkol\r\n";

    send(client->getSockfd(), rpl_welcome.c_str(), rpl_welcome.length(), 0);
    send(client->getSockfd(), rpl_yourhost.c_str(), rpl_yourhost.length(), 0);
    send(client->getSockfd(), rpl_created.c_str(), rpl_created.length(), 0);
    send(client->getSockfd(), rpl_myinfo.c_str(), rpl_myinfo.length(), 0);
}
