#include "include/IRC.h"
#include "srcs/server/Reactor.hpp"
#include "srcs/server/Server.hpp"

extern volatile sig_atomic_t serverIsRunning = 1;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>\n";
        return 1;
    }

    Reactor &reactor = Reactor::getInstance();
    Server   server(argv[1]);
    server.run();
    (void)reactor;

    return 0;
}
