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
    (void)c;
    std::string msg = std::string(":") + Reactor::getServerName() +
                      std::string(" 472 ") +
                      client->getUserInfo().getNickname() + " " +
                      std::string(1, c) + " :is unknown mode char to me\r\n";
    send(client->getSockfd(), msg.c_str(), msg.size(), 0);
}

static void sendErr1(int fd, const std::string& nick) {
    std::string msg = std::string(":") + Reactor::getServerName() +
                      std::string(" 461 ") + nick +
                      " MODE :Not enough parameters\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

static void sendErr2(int fd, const std::string& nick,
                     const std::string& nick2) {
    std::string msg = std::string(":") + Reactor::getServerName() +
                      std::string(" 401 ") + nick + " " + nick2 +
                      " :No such nick\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}
static void sendErr9(int fd, const std::string& nick, const std::string& nick2,
                     std::string channel) {
    std::string msg = std::string(":") + Reactor::getServerName() +
                      std::string(" 441 ") + nick + " " + nick2 + " " +
                      channel + " :They aren't on that channel\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

static void sendErr3(int fd, const std::string& nick,
                     const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getServerName() +
                      std::string(" 403 ") + nick + " " + channel +
                      " :No such channel\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}
static void sendErr4(int fd, const std::string& nick,
                     const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getServerName() +
                      std::string(" 442 ") + nick + " " + channel +
                      " :You're not on that channel\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

static void sendErr6(int fd, const std::string& nick,
                     const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getServerName() +
                      std::string(" 482 ") + nick + " " + channel +
                      " :You're not channel operator\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}
static void sendErr7(int fd, const std::string& nick, std::string str) {
    std::string msg = std::string(":") + Reactor::getServerName() +
                      std::string(" 461 ") + nick + "MODE" + str +
                      " :Not enough parameters\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}
static void sendErr8(int fd, const std::string& nick,
                     const std::string& channel) {
    std::string msg = std::string(":") + Reactor::getServerName() +
                      std::string(" 467 ") + nick + " " + channel +
                      " :Channel key already set\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

void handleKey(bool state, char c, std::vector<std::string>& tmp,
               Channel& channel, Client* client) {
    (void)c;
    if (tmp.empty()) {
        std::string str(1, (!state) * '-' + (state) * '+');
        return (sendErr7(client->getSockfd(),
                         client->getUserInfo().getNickname(), str + "k"));
    }
    std::string pass;
    if (tmp[0] != ":") {
        pass = tmp[0];
        tmp.erase(tmp.begin());
    } else {
        // tmp[0].erase(0, 1);
        tmp.erase(tmp.begin());
        pass = Utils::join(tmp);
        tmp.clear();
        // for (size_t i = 0; i < tmp.size(); i++) {
        //     pass += tmp[i];
        //     pass += " ";
        // }
        // pass.pop_back();
    }
    if (!state && channel.modeIsSet(CHANNEL_MODE::SET_KEY)) {
        if (pass != channel.getPassword())
            return (sendErr8(client->getSockfd(),
                             client->getUserInfo().getNickname(),
                             channel.name())); //: adrift.sg.quakenet.org 467 i1
                                               //: #ch :Channel key already set
        channel.unsetMode(CHANNEL_MODE::SET_KEY);
        std::string msg = ":" + client->getUserInfo().getNickname() + "!~" +
                          client->getUserInfo().getUsername() + "@" +
                          client->getIp() + " MODE " + channel.name() + " -k " +
                          pass + CR_LF;
        channel.sendToAll(client, msg);
        send(client->getSockfd(), msg.c_str(), msg.size(), 0);
    } else if (state && !channel.modeIsSet(CHANNEL_MODE::SET_KEY)) {
        channel.setPassword(pass);
        channel.setMode(CHANNEL_MODE::SET_KEY);
        std::string msg = ":" + client->getUserInfo().getNickname() + "!~" +
                          client->getUserInfo().getUsername() + "@" +
                          client->getIp() + " MODE " + channel.name() + " +k " +
                          pass + CR_LF;
        channel.sendToAll(client, msg);
        send(client->getSockfd(), msg.c_str(), msg.size(), 0);
    }
}

void handleInvite(bool state, char c, std::vector<std::string>& tmp,
                  Channel& channel, Client* client) {
    (void)c;
    (void)tmp;
    if (!state && channel.modeIsSet(CHANNEL_MODE::SET_INVITE_ONLY)) {
        channel.unsetMode(CHANNEL_MODE::SET_INVITE_ONLY);
        std::string msg = ":" + client->getUserInfo().getNickname() + "!~" +
                          client->getUserInfo().getUsername() + "@" +
                          client->getIp() + " MODE " + channel.name() +
                          " -i\r\n";
        channel.sendToAll(client, msg);
        send(client->getSockfd(), msg.c_str(), msg.size(), 0);
    } else if (state && !channel.modeIsSet(CHANNEL_MODE::SET_INVITE_ONLY)) {
        channel.setMode(CHANNEL_MODE::SET_INVITE_ONLY);
        std::string msg = ":" + client->getUserInfo().getNickname() + "!~" +
                          client->getUserInfo().getUsername() + "@" +
                          client->getIp() + " MODE " + channel.name() +
                          " +i\r\n";
        channel.sendToAll(client, msg);
        send(client->getSockfd(), msg.c_str(), msg.size(), 0);
    }
}

void handleOperator(bool state, char c, std::vector<std::string>& tmp,
                    Channel& channel, Client* client) {
    (void)c;
    if (tmp.empty()) {
        std::string str(1, (!state) * '-' + (state) * '+');
        return (sendErr7(client->getSockfd(),
                         client->getUserInfo().getNickname(), str + "o"));
    }
    std::string str;
    if (tmp[0] != ":") {
        str = tmp[0];
        tmp.erase(tmp.begin());
    } else {
        tmp.erase(tmp.begin());
        str = Utils::join(tmp);
        tmp.clear();
    }
    if (ClientList::exist(str) && !channel.exist(str))
        return (sendErr9(client->getSockfd(),
                         client->getUserInfo().getNickname(), str,
                         channel.name()));
    if (!ClientList::exist(str))
        return (sendErr2(client->getSockfd(),
                         client->getUserInfo().getNickname(), str));
    Client* user = channel.getClient(str);
    if (!state && channel.flagIsSet(user, MEMBER_PERMISSION::OPERATOR)) {
        channel.setPermission(user, MEMBER_PERMISSION::REGULAR);
        std::string msg = ":" + client->getUserInfo().getNickname() + "!~" +
                          client->getUserInfo().getUsername() + "@" +
                          client->getIp() + " MODE " + channel.name() + " -o " +
                          user->getUserInfo().getNickname() + CR_LF;
        channel.sendToAll(client, msg);
        send(client->getSockfd(), msg.c_str(), msg.size(), 0);
    } else if (state && !channel.flagIsSet(user, MEMBER_PERMISSION::OPERATOR)) {
        channel.setPermission(user, MEMBER_PERMISSION::OPERATOR);

        std::string msg = ":" + client->getUserInfo().getNickname() + "!~" +
                          client->getUserInfo().getUsername() + "@" +
                          client->getIp() + " MODE " + channel.name() + " +o " +
                          user->getUserInfo().getNickname() + CR_LF;
        channel.sendToAll(client, msg);
        send(client->getSockfd(), msg.c_str(), msg.size(), 0);
    }
}

void handleTopic(bool state, char c, std::vector<std::string>& tmp,
                 Channel& channel, Client* client) {
    (void)c;
    (void)tmp;
    if (!state && channel.modeIsSet(CHANNEL_MODE::SET_TOPIC)) {
        channel.unsetMode(CHANNEL_MODE::SET_TOPIC);
        std::string msg = ":" + client->getUserInfo().getNickname() + "!~" +
                          client->getUserInfo().getUsername() + "@" +
                          client->getIp() + " MODE " + channel.name() +
                          " -t\r\n";
        channel.sendToAll(client, msg);
        send(client->getSockfd(), msg.c_str(), msg.size(), 0);
    } else if (state && !channel.modeIsSet(CHANNEL_MODE::SET_TOPIC)) {
        channel.setMode(CHANNEL_MODE::SET_TOPIC);
        std::string msg = ":" + client->getUserInfo().getNickname() + "!~" +
                          client->getUserInfo().getUsername() + "@" +
                          client->getIp() + " MODE " + channel.name() +
                          " +t\r\n";
        channel.sendToAll(client, msg);
        send(client->getSockfd(), msg.c_str(), msg.size(), 0);
    }
}

void handleLimit(bool state, char c, std::vector<std::string>& tmp,
                 Channel& channel, Client* client) {
    (void)c;
    if (tmp.empty() && state)
        return (sendErr7(client->getSockfd(),
                         client->getUserInfo().getNickname(), "+l"));
    if (state && !channel.modeIsSet(CHANNEL_MODE::SET_LIMIT)) {
        std::string str;
        if (tmp[0] != ":") {
            str = tmp[0];
            tmp.erase(tmp.begin());
        } else {
            tmp.erase(tmp.begin()); //? why removing the first item
            str = Utils::join(tmp);
            tmp.clear();
        }
        std::stringstream ss(str);
        unsigned long     limit;
        ss >> limit;
        channel.setLimit(limit);
        channel.setMode(CHANNEL_MODE::SET_LIMIT);
        std::string msg = ":" + client->getUserInfo().getNickname() + "!~" +
                          client->getUserInfo().getUsername() + "@" +
                          client->getIp() + " MODE " + channel.name() + " +l " +
                          Utils::toStr(limit) + CR_LF;
        channel.sendToAll(client, msg);
        send(client->getSockfd(), msg.c_str(), msg.size(), 0);
    } else if (!state && channel.modeIsSet(CHANNEL_MODE::SET_LIMIT)) {
        channel.unsetMode(CHANNEL_MODE::SET_LIMIT);
        std::string msg = ":" + client->getUserInfo().getNickname() + "!~" +
                          client->getUserInfo().getUsername() + "@" +
                          client->getIp() + " MODE " + channel.name() +
                          " -l\r\n";
        channel.sendToAll(client, msg);
        send(client->getSockfd(), msg.c_str(), msg.size(), 0);
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
        (void)(*f[index])(state, mode[i], tmp, channel, client);
    }
    mode.erase(0, mode.find_first_of("+-"));
}

void MODE::execute(Client* client, const std::vector<std::string>& parameters) {
    std::vector<std::string> tmp(parameters);
    // for (size_t i = 0; i < parameters.size(); i++) {
    //     std::cerr << "para " + parameters[i] + "\n";
    // }
    if (tmp.size() < 2)
        return (
            sendErr1(client->getSockfd(), client->getUserInfo().getNickname()));
    if (tmp[0][0] != '#')
        return (sendErr3(client->getSockfd(),
                         client->getUserInfo().getNickname(), tmp[0]));
    if (!TChannels::exist(tmp[0]))
        return (sendErr3(client->getSockfd(),
                         client->getUserInfo().getNickname(), tmp[0]));
    Channel& tmpChannel = TChannels::channel(tmp[0]);
    tmp.erase(tmp.begin());
    if (!tmpChannel.exist(client))
        return (sendErr4(client->getSockfd(),
                         client->getUserInfo().getNickname(),
                         tmpChannel.name()));
    if (!tmpChannel.flagIsSet(client, MEMBER_PERMISSION::OPERATOR))
        return (sendErr6(client->getSockfd(),
                         client->getUserInfo().getNickname(),
                         tmpChannel.name()));
    std::string modes(tmp[0]);
    tmp.erase(tmp.begin());
    while (!modes.empty()) {
        handleflags(modes, tmp, tmpChannel, client);
    }
}