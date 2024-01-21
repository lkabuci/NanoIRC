#include "KICK.hpp"
#include <cstddef>
#include <stdexcept>
#include <string>

void KICK::execute(Client* client, const std::vector<std::string>& parameters) {
    std::vector<std::string> tmp(parameters);
    if (tmp.size() < 2)
        return (Reply::error(client->getSockfd(),
                             ERROR_CODES::ERR_NEEDMOREPARAMS, "KICK"));
    if (tmp[0][0] != '#')
        return (Reply::error(client->getSockfd(),
                             ERROR_CODES::ERR_UNKNOWNCOMMAND, "KICK"));
    tmp[0].erase(0, 1);
    if (!TChannels::exist(tmp[0]))
        return (Reply::error(client->getSockfd(),
                             ERROR_CODES::ERR_NOSUCHCHANNEL, tmp[0]));
    Channel tmpChannel = TChannels::channel(tmp[0]);
    tmp.erase(tmp.begin());
    if (!tmpChannel.exist(client))
        return (Reply::error(client->getSockfd(), ERROR_CODES::ERR_NOTONCHANNEL,
                             tmpChannel.name()));
    Client* user = tmpChannel.getClient(tmp[0]);
    if (!user)
        return (Reply::error(client->getSockfd(), ERROR_CODES::ERR_NOSUCHNICK,
                             user->getUserInfo().getNickname()));
    tmp.erase(tmp.begin());
    if (!tmpChannel.flagIsSet(client, MEMBER_PERMISSION::OPERATOR))
        return (Reply::error(client->getSockfd(),
                             ERROR_CODES::ERR_CHANOPRIVSNEEDED,
                             tmpChannel.name()));
    tmpChannel.remove(user);
    std::string reason;
    if (!tmp.empty()) {
        if (tmp[0][0] != ':')
            return (Reply::error(client->getSockfd(),
                                 ERROR_CODES::ERR_NEEDMOREPARAMS, "KICK"));
        for (size_t i = 0; i < tmp.size(); i++) {
            reason += tmp[i];
            reason += " ";
        }
        reason.pop_back();
        tmpChannel.sendToAll(client,
                             std::string(Reactor::getInstance().getServerIp()) +
                                 " " + "KICK #" + tmpChannel.name() + " " +
                                 user->getUserInfo().getUsername() + " " +
                                 reason + "\r\n");
    }
}
// :Operator!user@host KICK #channel TargetUser :Reason for the kick