#include "KICK.hpp"
#include <cstddef>
#include <string>

static void send1(int fd, const std::string& nick) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 461 ") + nick +
                      " KICK :Not enough parameters\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

static void send2(int fd, const std::string& nick, const std::string& nick2) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 401 ") + nick + " " + nick2 +
                      " :No such nick\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

static void send3(int fd, const std::string& nick, const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 403 ") + nick + " " + channel +
                      " :No such channel\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}
static void send4(int fd, const std::string& nick, const std::string nick2,
                  const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 441 ") + nick + " " + nick2 + " " +
                      channel + " :They aren't on that channel\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

static void send6(int fd, const std::string& nick, const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 482 ") + nick + " " + channel +
                      " :You're not channel operator\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

void KICK::execute(Client* client, const std::vector<std::string>& parameters) {
    std::vector<std::string> tmp(parameters);
    if (tmp.size() < 2)
        return (
            send1(client->getSockfd(), client->getUserInfo().getNickname()));
    if (tmp[0][0] != '#')
        return (send3(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmp[0]));
    if (!TChannels::exist(tmp[0]))
        return (send3(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmp[0]));
    Channel tmpChannel = TChannels::channel(tmp[0]);
    tmp.erase(tmp.begin());
    if (!tmpChannel.exist(client))
        return (send6(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmpChannel.name()));
    Client* user = tmpChannel.getClient(tmp[0]);
    if (!user)
        return (send4(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmp[0], tmpChannel.name()));
    tmp.erase(tmp.begin());
    if (!tmpChannel.flagIsSet(client, MEMBER_PERMISSION::OPERATOR))
        return (send6(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmpChannel.name()));
    tmpChannel.remove(user);
    std::string reason;
    if (!tmp.empty()) {
        if (tmp[0][0] != ':')
            reason = tmp[0];
        else {
            tmp[0].erase(0, 1);
            for (size_t i = 0; i < tmp.size(); i++) {
                reason += tmp[i];
                reason += " ";
            }
            reason.pop_back();
        }
    }
    if (reason.empty())
        reason = user->getUserInfo().getNickname();
    std::string msg = ":" + client->getUserInfo().getNickname() + "!~" +
                      client->getUserInfo().getUsername() + "@" +
                      Reactor::getInstance().getServerIp() + " KICK " +
                      tmpChannel.name() + " " +
                      user->getUserInfo().getNickname() + " :" + reason + CR_LF;
    tmpChannel.sendToAll(client, msg);
}