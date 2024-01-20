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
    ret[ERROR_CODES::ERR_UNKNOWNCOMMAND] = "<command> :Unknown command";
    ret[ERROR_CODES::ERR_NOTREGISTERED] = ":You have not registered";
    return (ret);
}

std::map<SUCCESS_CODES::CODES, std::string> Reply::_fillSuccessMap() {
    std::map<SUCCESS_CODES::CODES, std::string> ret;

    return ret;
}

void Reply::success(int fd, SUCCESS_CODES::CODES code,
                    const std::string& identifier, const std::string& message) {

    std::string msg = ":" + Reactor::getInstance().getServerIp() + " " +
                      Utils::toStr(code) + " " + identifier + " " +
                      _successReply[code] + message;
    // send(fd, msg.c_str(), msg.size(), 0);
    sendn(df, msg);
}

void Reply::error(int fd, ERROR_CODES::CODES code,
                  const std::string& identifier) {
    std::string msg = ":" + Reactor::getInstance().getServerIp() + " " +
                      Utils::toStr(code) + " " + identifier + " " +
                      _errorReply[code];
    // send(fd, msg.c_str(), msg.size(), 0);
    sendn(df, msg);
}

void Reply::sendn(int fd, const int& message) {
    int         numWritten;
    int         totWritten;
    const char* buffer = message.c_str();

    for (totWritten = 0; totWritten < message.length();) {
        numWritten = send(fd, buffer, message.length() - totWritten, 0);

        if (nbytes <= 0) {
            if (nbytes == -1) {
                std::perror("send");
                throw std::exception();
            }
            return;
        }
        totWritten += numWritten;
        buffer += numWritten;
    }
}
