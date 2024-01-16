#include <iostream>
#include "include/IRC.h"
#include "srcs/server/Reactor.hpp"
#include "srcs/server/Server.hpp"
#include "srcs/parser/Message.hpp"

extern volatile sig_atomic_t serverIsRunning = 1;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>\n";
        return 1;
    }

    Reactor &reactor = Reactor::getInstance();
    Server   server(argv[1]);
    try {
        server.run();
    }catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    (void)reactor;
    return 0;
}
