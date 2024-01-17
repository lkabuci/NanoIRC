#include "TOPIC.hpp"
#include <cstddef>
#include <stdexcept>
#include <vector>

void TOPIC::execute(Client*                         client,
                    const std::vector<std::string>& parameters) {
    std::vector<std::string> tmp(parameters);
    if (tmp.size() < 1)
        throw std::runtime_error("");
    if (tmp[0][0] != '#')
        throw std::runtime_error("channel !!?");
    tmp[0].erase(0);
    if (!TChannels::exist(tmp[0]))
        throw std::runtime_error("Channel not found a sahbi");
    Channel tmpChannel = TChannels::channel(tmp[0]);
    tmp.erase(tmp.begin());
    if (tmp.empty()) {
        std::string topic = tmpChannel.getTopic();
        throw std::runtime_error(topic);
    } else {
        if (tmp[0][0] != ':')
            throw std::runtime_error(": makaynach");
        tmp[0].erase(0);
        std::string topic;
        for (size_t i = 0; i < tmp.size(); i++) {
            topic += tmp[i];
            topic += " ";
        }
        topic.pop_back();
        tmpChannel.setTopic(topic);
    }
}