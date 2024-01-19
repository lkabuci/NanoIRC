#include "../include/IRC.h"
#include "Utils/SignalHandler.hpp"
#include "parser/Message.hpp"
#include "server/Reactor.hpp"
#include "server/Server.hpp"

extern volatile sig_atomic_t serverIsRunning = 1;

static void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " <port> <password>\n";
    std::exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printUsage(argv[0]);
    }
    if (!Utils::isAllDigits(argv[1])) {
        std::cerr << "Error: Port must be a valid number.\n";
        printUsage(argv[0]);
    }
    SignalHandler signalHandler;
    Reactor&      reactor = Reactor::getInstance();
    Server        server(argv[1], argv[2]);
    try {
        server.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    (void)reactor;
    return 0;
}
