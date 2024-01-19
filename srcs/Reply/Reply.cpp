#include "Reply.hpp"

std::map<SUCCESS_CODES::CODES, std::string> Reply::_successReply =
    _fillSuccessMap();
std::map<ERROR_CODES::CODES, std::string> Reply::_errorReply = _fillErrorMap();

std::map<ERROR_CODES::CODES, std::string> Reply::_fillErrorMap() {
    std::map<ERROR_CODES::CODES, std::string> ret;

    ret[ERROR_CODES::ERR_ALREADYREGISTRED] = ":You may not reregister";
    ret[ERROR_CODES::ERR_NEEDMOREPARAMS] = "<command> :Not enough parameters";
    ret[ERROR_CODES::ERR_NONICKNAMEGIVEN] = ":No nickname given";
    ret[ERROR_CODES::ERR_NICKNAMEINUSE] = ":Nickname is already in use";
    ret[ERROR_CODES::ERR_NOSUCHCHANNEL] = ":No such channel";
    ret[ERROR_CODES::ERR_INVITEONLYCHAN] = ":Cannot join channel (+i)";
    ret[ERROR_CODES::ERR_BADCHANNELKEY] = ":Cannot join channel (+k)";
    ret[ERROR_CODES::ERR_NORECIPIENT] = ":No recipient given (<command>)";
    ret[ERROR_CODES::ERR_NOTEXTTOSEND] = ":No text to send";
    ret[ERROR_CODES::ERR_TOOMANYTARGETS] =
        "<target> :Duplicate recipients. No message delivered";
    ret[ERROR_CODES::ERR_NOSUCHNICK] = "<nickname> :No such nick/channel";
    ret[ERROR_CODES::ERR_NICKCOLLISION] = "<nick> :Nickname collision KILL";
    return (ret);
}

std::map<SUCCESS_CODES::CODES, std::string> Reply::_fillSuccessMap() {
    std::map<SUCCESS_CODES::CODES, std::string> ret;

    return ret;
}

void Reply::success(int fd, const std::string& servername,
                    const std::string&              nickname,
                    const std::vector<std::string>& message) {
    //      join #toast,#ircv3 mysecret
    //      S <-   :alice!~a@localhost JOIN #toast
    std::string reply = std::string(":") + nickname + "@" + servername + " " +
                        Utils::join(message);

    send(fd, reply.c_str(), reply.length(), 0);
}

void Reply::error(int fd, ERROR_CODES::CODES code,
                  const std::string& servername, const std::string& nickname,
                  std::vector<std::string>& parameters) {
    //: irc.example.com 471 alice #test :Cannot join channel (+l)
    std::string reply = std::string(":") + servername + " " +
                        Utils::toStr(code) + nickname +
                        Utils::join(parameters) + ":" + _errorReply[code];

    send(fd, reply.c_str(), reply.length(), 0);
}
