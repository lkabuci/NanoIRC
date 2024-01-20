#include "Utils.hpp"
#include <cstdlib>
#include <sstream>

std::string Utils::join(const std::vector<std::string>& arr) {
    if (arr.empty())
        return "";
    if (arr.size() == 1)
        return arr[0];
    std::string str;
    size_t      i = 1;

    str.append(arr[0]);
    do {
        str.append(" ");
        str.append(arr[i]);
        i++;
    } while (i < arr.size());
    return str;
}

std::string Utils::toStr(int nbr) {
    std::string        ret;
    std::ostringstream convert;
    convert << nbr;
    ret = convert.str();
    return ret;
}

bool Utils::isAllDigits(const char* str) {
    char* endptr;
    std::strtol(str, &endptr, 10);
    return *endptr == 0 && *str != 0;
}

std::string Utils::getNickname(const std::string& str) {
    if (!std::isdigit(str[0]))
        throw std::exception();
    if (str.find_first_not_of(
            "abcdefghijklmnopqrstuvwxyz0123456789-[]\\`^{}") !=
        std::string::npos)
        throw std::exception();
    return std::string(str);
}
