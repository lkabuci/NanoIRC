//
// Created by Redouane Elkaboussi on 1/15/24.
//

#include "Demultiplexer.hpp"

int Demultiplexer::waitForEvents() {
    std::vector<pollfd>& pfds = Reactor::getInstance().getPollfds();
    return poll(pfds.data(), pfds.size(), -1);
}
