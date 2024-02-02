#include "INVITE.hpp"
#include <vector>

static void send1(int fd, const std::string& nick) {
    std::string msg = std::string(":") + Reactor::getServerName() +
                      std::string(" 461 ") + nick +
                      " INVITE :Not enough parameters\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

static void send2(int fd, const std::string& nick, const std::string& nick2) {
    std::string msg = std::string(":") + Reactor::getServerName() +
                      std::string(" 401 ") + nick + " " + nick2 +
                      " :No such nick\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

static void send3(int fd, const std::string& nick, const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getServerName() +
                      std::string(" 403 ") + nick + " " + channel +
                      " :No such channel\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}
static void send4(int fd, const std::string& nick, const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getServerName() +
                      std::string(" 442 ") + nick + " " + channel +
                      " :You're not on that channel\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}
static void send5(int fd, const std::string& nick) {
    std::string msg = std::string(":") + Reactor::getServerName() +
                      std::string(" 401 ") + nick + " INVITE :No such nick\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}
static void send6(int fd, const std::string& nick, const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getServerName() + " 482 " +
                      nick + " " + channel +
                      " :You're not channel operator\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

static void sendSucc(int fd, const std::string& nick, const std::string& nick2,
                     const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getServerName() + " 341 " +
                      nick + " " + nick2 + " " + channel + CR_LF;
    send(fd, msg.c_str(), msg.size(), 0);
}
static void SendInv(int fd, const std::string& nick, const std::string& user,
                    const std::string& nick2, const std::string& channel,
                    const std::string& clientIp) {
    std::string msg = std::string(":") + nick + "!~" + user + "@" + clientIp +
                      " INVITE " + nick2 + " " + channel + CR_LF;
    send(fd, msg.c_str(), msg.size(), 0);
}

void INVITE::execute(Client*                         client,
                     const std::vector<std::string>& parameters) {
    std::vector<std::string> tmp(parameters);

    if (tmp.size() < 2)
        return (
            send1(client->getSockfd(), client->getUserInfo().getNickname()));
    Client* inv = ClientList::get(tmp[0]);
    tmp.erase(tmp.begin());
    if (!inv)
        return (send2(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmp[0]));
    if (tmp[0][0] != '#')
        return (send3(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmp[0]));
    if (!TChannels::exist(tmp[0]))
        return (send3(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmp[0]));
    Channel& tmpChannel = TChannels::channel(tmp[0]);
    tmp.erase(tmp.begin());
    if (!tmpChannel.exist(client))
        return (send4(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmpChannel.name()));
    else if (tmpChannel.exist(inv))
        return (
            send5(client->getSockfd(), client->getUserInfo().getNickname()));
    else if (tmpChannel.modeIsSet(CHANNEL_MODE::SET_INVITE_ONLY) &&
             !tmpChannel.flagIsSet(client, MEMBER_PERMISSION::OPERATOR))
        return (send6(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmpChannel.name()));
    tmpChannel.invite(inv);
    sendSucc(client->getSockfd(), client->getUserInfo().getNickname(),
             inv->getUserInfo().getNickname(), tmpChannel.name());
    SendInv(inv->getSockfd(), client->getUserInfo().getNickname(),
            client->getUserInfo().getUsername(),
            inv->getUserInfo().getNickname(), tmpChannel.name(), inv->getIp());
}