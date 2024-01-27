#include "TOPIC.hpp"
#include <cstddef>
#include <string>
#include <vector>

static void sendErr1(int fd, const std::string& nick) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 461 ") + nick +
                      " TOPIC :Not enough parameters\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

static void sendErr2(int fd, const std::string& nick,
                     const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 403 ") + nick + " " + channel +
                      " :No such channel\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}
static void sendErr3(int fd, const std::string& nick,
                     const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 442 ") + nick + " " + channel +
                      " :You're not on that channel\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}
static void sendErr4(int fd, const std::string& nick,
                     const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 331 ") + nick + " " + channel +
                      " :No topic is set.\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

static void sendErr5(int fd, const std::string& nick,
                     const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 482 ") + nick + " " + channel +
                      " :You're not channel operator\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}
static void sendSucc(int fd, const std::string& nick,
                     const std::string& channel, const std::string& topic) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      " 332 " + nick + " " + channel + " :" + topic + CR_LF;
    send(fd, msg.c_str(), msg.size(), 0);
}
static void sendSucc2(int fd, const std::string& nick,
                      const std::string& channel, const std::string& nickSetter,
                      std::string time) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      " 333 " + nick + " " + channel + " " + nickSetter + " " +
                      time + CR_LF;
    send(fd, msg.c_str(), msg.size(), 0);
}

void TOPIC::execute(Client*                         client,
                    const std::vector<std::string>& parameters) {
    std::vector<std::string> tmp(parameters);
    std::string              topic;
    if (tmp.size() < 1)
        return (
            sendErr1(client->getSockfd(), client->getUserInfo().getNickname()));
    if (tmp[0][0] != '#')
        return (sendErr2(client->getSockfd(),
                         client->getUserInfo().getNickname(), tmp[0]));
    if (!TChannels::exist(tmp[0]))
        return (sendErr2(client->getSockfd(),
                         client->getUserInfo().getNickname(), tmp[0]));
    Channel tmpChannel = TChannels::channel(tmp[0]);
    if (!tmpChannel.exist(client))
        return (sendErr3(client->getSockfd(),
                         client->getUserInfo().getNickname(),
                         tmpChannel.name()));
    tmp.erase(tmp.begin());
    if (tmp.empty()) {
        topic = tmpChannel.getTopic();
        if (topic.empty())
            return (sendErr4(client->getSockfd(),
                             client->getUserInfo().getNickname(),
                             tmpChannel.name()));
        sendSucc(client->getSockfd(), client->getUserInfo().getNickname(),
                 tmpChannel.name(), topic);
        sendSucc2(client->getSockfd(), client->getUserInfo().getNickname(),
                  tmpChannel.name(),
                  tmpChannel.getTopicSetter()->getUserInfo().getNickname(),
                  Utils::toStr(tmpChannel.getTime()));
    } else {
        if (tmp[0][0] != ':') {
            topic = tmp[0];
        } else {
            tmp[0].erase(0, 1);
            for (size_t i = 0; i < tmp.size(); i++) {
                topic += tmp[i];
                topic += " ";
            }
            topic.pop_back();
        }
        if (tmpChannel.flagIsSet(client, MEMBER_PERMISSION::OPERATOR) ||
            tmpChannel.modeIsSet(CHANNEL_MODE::SET_TOPIC)) {
            tmpChannel.setTopic(topic);
            std::string msg = ":" + client->getUserInfo().getNickname() + "!~" +
                              client->getUserInfo().getUsername() + "@" +
                              Reactor::getInstance().getServerIp() + " TOPIC " +
                              tmpChannel.name() + ":" + topic + CR_LF;
            tmpChannel.setTopicSetter(client);
            tmpChannel.setTime(time(NULL));
            tmpChannel.sendToAll(client, msg);
        } else
            return (sendErr5(client->getSockfd(),
                             client->getUserInfo().getNickname(),
                             tmpChannel.name()));
    }
}