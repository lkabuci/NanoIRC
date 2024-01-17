#include "KICK.hpp"
#include <cstddef>
#include <stdexcept>
#include <string>

void KICK::execute(Client* client, const std::vector<std::string>& parameters) {
    std::vector<std::string> tmp(parameters);
    if (tmp.size() < 2)
        throw std::runtime_error("invalid args");
    if (tmp[0][0] != '#')
        throw std::runtime_error("Invalid Channel");
    tmp[0].erase(0);
    if (!TChannels::exist(tmp[0]))
        throw std::runtime_error("Channel not found a sahbi");
    Channel tmpChannel = TChannels::channel(tmp[0]);
    tmp.erase(tmp.begin());
    if (!tmpChannel.exist(client))
        throw std::runtime_error("You Re not a member in the channel");
    Client* user = tmpChannel.getClient(tmp[0]);
    if (!user)
        throw std::runtime_error(std::string("client : ") + tmp[0] +
                                 std::string("not found in #") + tmp[0]);
    tmp.erase(tmp.begin());
    if (!tmpChannel.flagIsSet(client, MEMBER_PERMISSION::OPERATOR))
        throw std::runtime_error(client->getUserInfo().getUsername() +
                                 ": Cannot kick he is not operator");
    tmpChannel.remove(user);
    if (!tmp.empty()) {
        if (tmp[0][0] != ':')
            throw std::runtime_error("Syntax reason error need \':\'");
        tmp[0].erase(0);
        std::string reason;
        for (size_t i = 0; i < tmp.size(); i++) {
            reason += tmp[i];
            reason += " ";
        }
        reason.pop_back();
    }
}