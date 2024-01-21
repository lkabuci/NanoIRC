#include "INVITE.hpp"
#include <vector>

void INVITE::execute(Client*                         client,
                     const std::vector<std::string>& parameters) {
    std::vector<std::string> tmp(parameters);

    if (tmp.size() < 2)
        return (Reply::error(client->getSockfd(),
                             ERROR_CODES::ERR_NEEDMOREPARAMS, "INVITE"));
    Client* inv = ClientList::get(tmp[0]);
    if (!inv)
        return (Reply::error(client->getSockfd(), ERROR_CODES::ERR_NOSUCHNICK,
                             tmp[0]));
    tmp.erase(tmp.begin());
    if (tmp[0][0] != '#')
        return (Reply::error(client->getSockfd(),
                             ERROR_CODES::ERR_UNKNOWNCOMMAND, "INVITE"));
    tmp[0].erase(0, 1);
    if (!TChannels::exist(tmp[0]))
        return (Reply::error(client->getSockfd(),
                             ERROR_CODES::ERR_NOSUCHCHANNEL, tmp[0]));
    Channel tmpChannel = TChannels::channel(tmp[0]);
    tmp.erase(tmp.begin());
    if (!tmpChannel.exist(client))
        return (Reply::error(client->getSockfd(), ERROR_CODES::ERR_NOTONCHANNEL,
                             tmpChannel.name()));
    else if (tmpChannel.exist(inv))
        return (Reply::error(
            client->getSockfd(), ERROR_CODES::ERR_USERONCHANNEL,
            inv->getUserInfo().getUsername() + " " + tmpChannel.name()));

    else if (tmpChannel.isInvited(inv))
        return;
    else if (tmpChannel.modeIsSet(CHANNEL_MODE::SET_INVITE_ONLY) &&
             !tmpChannel.flagIsSet(client, MEMBER_PERMISSION::OPERATOR))
        return (Reply::error(client->getSockfd(),
                             ERROR_CODES::ERR_CHANOPRIVSNEEDED,
                             tmpChannel.name()));
    tmpChannel.invite(inv);
    Reply::success(client->getSockfd(), SUCCESS_CODES::RPL_INVITING,
                   tmpChannel.name() + " " + inv->getUserInfo().getNickname(),
                   "");
    Reply::success(
        inv->getSockfd(), SUCCESS_CODES::RPL_INVITING,
        tmpChannel.name() + " " + client->getUserInfo().getNickname(), "");
}