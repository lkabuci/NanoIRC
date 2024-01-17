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

};
}

class Reply {
  public:
    static void succes(int fd, SUCCESS_CODES::CODES code,
                       const std::string&              servername,
                       const std::string&              nickname,
                       const std::vector<std::string>& message);
    static void error(int fd, ERROR_CODES::CODES code,
                      const std::string&        servername,
                      const std::string&        nickname,
                      std::vector<std::string>& parameters);

  private:
    Reply();
    static std::map<SUCCESS_CODES::CODES, std::string> _fillSuccessMap();
    static std::map<ERROR_CODES::CODES, std::string>   _fillErrorMap();

    static std::map<SUCCESS_CODES::CODES, std::string> _successReply;
    static std::map<ERROR_CODES::CODES, std::string>   _errorReply;
};

#endif