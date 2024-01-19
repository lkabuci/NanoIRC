#include "MODE.hpp"
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <vector>

void error(bool state, std::vector<std::string>& tmp, Channel& channel) {
    (void)state;
    (void)tmp;
    (void)channel;
    throw std::runtime_error("invalid identifier");
}

void handleKey(bool state, std::vector<std::string>& tmp, Channel& channel) {
    if (tmp.empty())
        return (std::cout << "flag k need a valid key", (void)0);
    std::string pass = tmp[0];
    tmp.erase(tmp.begin());
    if (!state && channel.modeIsSet(CHANNEL_MODE::SET_KEY)) {
        if (pass != channel.getPassword())
            return (std::cout << "Wrong key: " + pass, (void)0);
        channel.unsetMode(CHANNEL_MODE::SET_KEY);
    } else if (state && !channel.modeIsSet(CHANNEL_MODE::SET_KEY)) {
        channel.setPassword(pass);
        channel.setMode(CHANNEL_MODE::SET_KEY);
    }
}

void handleInvite(bool state, std::vector<std::string>& tmp, Channel& channel) {
    if (!state && channel.modeIsSet(CHANNEL_MODE::SET_INVITE_ONLY))
        channel.unsetMode(CHANNEL_MODE::SET_INVITE_ONLY);
    if (state && !channel.modeIsSet(CHANNEL_MODE::SET_INVITE_ONLY))
        channel.setMode(CHANNEL_MODE::SET_INVITE_ONLY);
}

void handleOperator(bool state, std::vector<std::string>& tmp,
                    Channel& channel) {
    if (tmp.empty())
        return (std::cout << "syntax error" << std::endl, (void)0);
    Client *user = channel.getClient(tmp[0]);
    if(!user)
        return (std::cout << "No such nick" + tmp[0] << std::endl, (void)0);
    tmp.erase(tmp.begin());
    // if(!state && channel.flagIsSet(user, MEMBER_PERMISSION::OPERATOR))
    //     channel.


}

void handleTopic(bool state, std::vector<std::string>& tmp, Channel& channel) {
    if (!state && channel.modeIsSet(CHANNEL_MODE::SET_TOPIC))
        channel.unsetMode(CHANNEL_MODE::SET_TOPIC);
    if (state && !channel.modeIsSet(CHANNEL_MODE::SET_TOPIC))
        channel.setMode(CHANNEL_MODE::SET_TOPIC);
}

void handleLimit(bool state, std::vector<std::string>& tmp, Channel& channel) {
    if (tmp.empty())
        return (std::cout << "syntax error" << std::endl, (void)0);
    std::stringstream ss(tmp[0]);
    tmp.erase(tmp.begin());
    unsigned long limit;
    ss >> limit;
    if (!ss.eof()) {
        return (std::cout << "LIMIT ERROR" << std::endl, (void)0);
    }
}

void handleflags(std::string& mode, std::vector<std::string>& tmp,
                 Channel& channel) {
    bool state;
    if (mode[0] == '-' || mode[0] == '+')
        state = (mode[0] == '+');
    else
        throw std::runtime_error("looking for '+' or '-' ");
    mode.erase(0, 1);
    for (size_t i = 0; i < mode.size() && !std::strchr("-+", mode[i]); i++) {
        void (*f[])(bool state, std::vector<std::string>& tmp,
                    Channel& channel) = {&error,        &handleKey,
                                         &handleInvite, &handleOperator,
                                         &handleTopic,  &handleLimit};
        int  index = (mode[i] == 'k') * 1 + (mode[i] == 'i') * 2 +
                    (mode[i] == 'o') * 3 + (mode[i] == 't') * 4 +
                    (mode[i] == 'l') * 5;
        (void)(*f[i])(state, tmp, channel);
    }
    mode.erase(0, mode.find_first_of("+-"));
}

void MODE::execute(Client* client, const std::vector<std::string>& parameters) {
    std::vector<std::string> tmp(parameters);
    if (tmp.size() < 2)
        throw std::runtime_error("invalid args");
    if (tmp[0][0] != '#')
        throw std::runtime_error("need a channel");
    tmp[0].erase(0, 1);
    if (!TChannels::exist(tmp[0]))
        throw std::runtime_error("channel mkynach");
    Channel tmpChannel = TChannels::channel(tmp[0]);
    tmp.erase(tmp.begin());
    if (!tmpChannel.exist(client) ||
        !tmpChannel.flagIsSet(client, MEMBER_PERMISSION::OPERATOR))
        throw std::runtime_error("m3ndkch l7a9");
    std::string modes(tmp[0]);
    tmp.erase(tmp.begin());
    while (!modes.empty()) {
        handleflags(modes, tmp, tmpChannel);
    }
}