#include "INVITE.hpp"
#include <stdexcept>
#include <vector>

void INVITE::execute(Client*                         client,
                     const std::vector<std::string>& parameters) {
    std::vector<std::string> tmp(parameters);
    if (tmp.size() != 2)
        throw std::runtime_error("invalid args");
    Client* inv = Clients::get(tmp[0]);
    if (!inv)
        throw std::runtime_error("Client makaynch");
    tmp.erase(tmp.begin());
    if (tmp[0][0] != '#')
        throw std::runtime_error("Channel needed");
    tmp[0].erase(0);
    if (!TChannels::exist(tmp[0]))
        throw std::runtime_error("Channel not found a sahbi");
    Channel tmpChannel = TChannels::channel(tmp[0]);
    tmp.erase(tmp.begin());
    if (!tmpChannel.exist(client))
        throw std::runtime_error("You Re not a member in the channel");
    tmpChannel.invite(inv);
}