#include "Reply.hpp"

std::map<SUCCESS_CODES::CODES, std::string> Reply::_successReply =
    _fillSuccessMap();
std::map<ERROR_CODES::CODES, std::string> Reply::_errorReply = _fillErrorMap();

std::map<ERROR_CODES::CODES, std::string> Reply::_fillErrorMap() {
    std::map<ERROR_CODES::CODES, std::string> ret;
    // ret[1] = "zabba w chta sabba";

    return (ret);
}

std::map<SUCCESS_CODES::CODES, std::string> Reply::_fillSuccessMap() {
    std::map<SUCCESS_CODES::CODES, std::string> ret;

    return ret;
}

void Reply::succes(int fd, SUCCESS_CODES::CODES code,
                   const std::string& servername, const std::string& nickname,
                   const std::vector<std::string>& message) {
    //      join #toast,#ircv3 mysecret
    //      S <-   :alice!~a@localhost JOIN #toast
    std::string reply = std::string(":") + nickname + "@" + servername + " " +
                        Utils::join(message);
    ;

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
