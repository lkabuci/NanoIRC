#include "MODE.hpp"
#include <stdexcept>
#include <vector>

void MODE::execute(Client* client, const std::vector<std::string>& parameters) {
    std::vector<std::string> tmp(parameters);
    if (tmp.size() < 2)
        throw std::runtime_error("invalid args");
}