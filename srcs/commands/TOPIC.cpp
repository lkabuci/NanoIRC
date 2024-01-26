#include "TOPIC.hpp"
#include <cstddef>
#include <stdexcept>
#include <vector>

void send1(int fd, const std::string& nick) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 461 ") + nick +
                      " TOPIC :Not enough parameters\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

void send3(int fd, const std::string& nick, const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 403 ") + nick + " #" + channel +
                      " :No such channel\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}
void send4(int fd, const std::string& nick, const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 442 ") + nick + " #" + channel +
                      " :You're not on that channel\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

void TOPIC::execute(Client*                         client,
                    const std::vector<std::string>& parameters) {
    std::vector<std::string> tmp(parameters);
    std::string              topic;
    if (tmp.size() < 1)
        return (
            send1(client->getSockfd(), client->getUserInfo().getNickname()));
    if (tmp[0][0] != '#')
        return (send3(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmp[0].substr(1)));
    tmp[0].erase(0, 1);
    if (!TChannels::exist(tmp[0]))
        return (send3(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmp[0]));
    Channel tmpChannel = TChannels::channel(tmp[0]);
    if (!tmpChannel.exist(client))
        return (send4(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmpChannel.name()));
    tmp.erase(tmp.begin());
    if (tmp.empty()) {
        topic = tmpChannel.getTopic();
        return;
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
            tmpChannel.modeIsSet(CHANNEL_MODE::SET_TOPIC))
            tmpChannel.setTopic(topic);
        else
            throw std::runtime_error("cannot set topic");
    }
}