#include "Utils.hpp"

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
