#include "MODE.hpp"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

void error(bool state, char c, std::vector<std::string>& tmp, Channel& channel,
           Client* client) {
    (void)state;
    (void)tmp;
    (void)channel;
    (void)client;
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 472 ") +
                      client->getUserInfo().getNickname() + " " +
                      std::string(1, c) + " :is unknown mode char to me\r\n";
}

void send1(int fd, const std::string& nick) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 461 ") + nick +
                      " MODE :Not enough parameters\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

void send2(int fd, const std::string& nick, const std::string& nick2) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 401 ") + nick + " " + nick2 +
                      " :No such nick\r\n";
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

void send6(int fd, const std::string& nick, const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 482 ") + nick + " #" + channel +
                      " :You're not channel operator\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}
void send7(int fd, const std::string& nick, const std::string& channel,
           std::string str) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 461 ") + nick + "MODE" + str +
                      " :Not enough parameters\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}
void send8(int fd, const std::string& nick, const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getInstance().getServerIp() +
                      std::string(" 467 ") + nick + " #" + channel +
                      " :Channel key already set\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

void handleKey(bool state, char c, std::vector<std::string>& tmp,
               Channel& channel, Client* client) {
    if (tmp.empty()) {
        std::string str(1, (!state) * '-' + (state) * '+');
        return (send7(client->getSockfd(), client->getUserInfo().getNickname(),
                      channel.name(), str + "k"));
    }
    std::string pass = tmp[0];
    tmp.erase(tmp.begin());
    if (!state && channel.modeIsSet(CHANNEL_MODE::SET_KEY)) {
        if (pass != channel.getPassword())
            return (send8(client->getSockfd(),
                          client->getUserInfo().getNickname(),
                          channel.name())); //: adrift.sg.quakenet.org 467 i1
                                            //: #ch :Channel key already set
        channel.unsetMode(CHANNEL_MODE::SET_KEY);
    } else if (state && !channel.modeIsSet(CHANNEL_MODE::SET_KEY)) {
        channel.setPassword(pass);
        channel.setMode(CHANNEL_MODE::SET_KEY);
    }
}

void handleInvite(bool state, char c, std::vector<std::string>& tmp,
                  Channel& channel, Client* client) {
    if (!state && channel.modeIsSet(CHANNEL_MODE::SET_INVITE_ONLY))
        channel.unsetMode(CHANNEL_MODE::SET_INVITE_ONLY);
    if (state && !channel.modeIsSet(CHANNEL_MODE::SET_INVITE_ONLY))
        channel.setMode(CHANNEL_MODE::SET_INVITE_ONLY);
}

void handleOperator(bool state, char c, std::vector<std::string>& tmp,
                    Channel& channel, Client* client) {
    if (tmp.empty()) {
        std::string str(1, (!state) * '-' + (state) * '+');
        return (send7(client->getSockfd(), client->getUserInfo().getNickname(),
                      channel.name(), str + "o"));
    }
    Client* user = channel.getClient(tmp[0]);
    if (!user)
        return (send2(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmp[0]));
    tmp.erase(tmp.begin());
    if (!state && channel.flagIsSet(user, MEMBER_PERMISSION::OPERATOR))
        channel.setPermission(user, MEMBER_PERMISSION::REGULAR);
    if (state && !channel.flagIsSet(user, MEMBER_PERMISSION::OPERATOR))
        channel.setPermission(user, MEMBER_PERMISSION::OPERATOR);
}

void handleTopic(bool state, char c, std::vector<std::string>& tmp,
                 Channel& channel, Client* client) {
    (void)tmp;
    if (!state && channel.modeIsSet(CHANNEL_MODE::SET_TOPIC))
        channel.unsetMode(CHANNEL_MODE::SET_TOPIC);
    if (state && !channel.modeIsSet(CHANNEL_MODE::SET_TOPIC))
        channel.setMode(CHANNEL_MODE::SET_TOPIC);
}

void handleLimit(bool state, char c, std::vector<std::string>& tmp,
                 Channel& channel, Client* client) {
    if (tmp.empty() && state)
        return (send7(client->getSockfd(), client->getUserInfo().getNickname(),
                      channel.name(), "+l"));
    if (state) {
        std::stringstream ss(tmp[0]);
        tmp.erase(tmp.begin());
        unsigned long limit;
        ss >> limit;
        if (!limit)
            return;
        channel.setLimit(limit);
        channel.setMode(CHANNEL_MODE::SET_LIMIT);
    } else {
        channel.unsetMode(CHANNEL_MODE::SET_LIMIT);
    }
}

void handleflags(std::string& mode, std::vector<std::string>& tmp,
                 Channel& channel, Client* client) {
    bool state = true;
    if (mode[0] == '-' || mode[0] == '+')
        state = (mode[0] == '+');
    else
        error(state, mode[0], tmp, channel, client);
    mode.erase(0, 1);
    for (size_t i = 0; i < mode.size() && !std::strchr("-+", mode[i]); i++) {
        void (*f[])(bool state, char c, std::vector<std::string>& tmp,
                    Channel& channel, Client* client) = {
            &error,          &handleKey,   &handleInvite,
            &handleOperator, &handleTopic, &handleLimit};
        int index = (mode[i] == 'k') * 1 + (mode[i] == 'i') * 2 +
                    (mode[i] == 'o') * 3 + (mode[i] == 't') * 4 +
                    (mode[i] == 'l') * 5;
        (void)(*f[i])(state, mode[i], tmp, channel, client);
    }
    mode.erase(0, mode.find_first_of("+-"));
}

void MODE::execute(Client* client, const std::vector<std::string>& parameters) {
    std::vector<std::string> tmp(parameters);
    if (tmp.size() < 2)
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
    tmp.erase(tmp.begin());
    if (!tmpChannel.exist(client))
        return (send4(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmpChannel.name()));
    if (!tmpChannel.flagIsSet(client, MEMBER_PERMISSION::OPERATOR))
        return (send6(client->getSockfd(), client->getUserInfo().getNickname(),
                      tmpChannel.name()));
    std::string modes(tmp[0]);
    tmp.erase(tmp.begin());
    while (!modes.empty()) {
        handleflags(modes, tmp, tmpChannel, client);
    }
}