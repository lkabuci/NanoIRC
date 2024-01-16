#include "ErrorReplies.hpp"

std::map<ERROR_CODES::Codes, std::string>
    ErrorReplies::_values(_init_values_map());

std::map<ERROR_CODES::Codes, std::string> ErrorReplies::_init_values_map() {
    std::map<ERROR_CODES::Codes, std::string> m;

    m[ERROR_CODES::ERR_UNKNOWNCOMMAND] = "421";
    m[ERROR_CODES::ERR_NEEDMOREPARAMS] = "461";
    m[ERROR_CODES::ERR_ALREADYREGISTRED] = "462";
    m[ERROR_CODES::ERR_PASSWDMISMATCH] = "464";
    return m;
}

std::string ErrorReplies::describe(const ERROR_CODES::Codes& code,
                                   const std::string&        str) {
    if (_values.find(code) != _values.end())
        return str + _values[code];
    throw std::runtime_error("Unknown code.");
}

void ErrorReplies::reply(int fd, const std::string& sender,
                         const ERROR_CODES::Codes& code,
                         const std::string&        target) {
    std::string to_send;

    if (_values.find(code) == _values.end())
        throw std::runtime_error("Unknown code.");

    to_send = sender + " " + _values[code] + " " + target;
    write(fd, to_send.c_str(), to_send.length());
}