#include "../include/IRC.h"
#include "Utils/SignalHandler.hpp"
#include "server/Server.hpp"

volatile sig_atomic_t serverIsRunning = 1;

static void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " <port> <password>\n";
    std::exit(EXIT_FAILURE);
}

void f() {
    std::exit(1);
}

int main(int argc, char* argv[]) {
    std::set_new_handler(f);
    if (argc != 3) {
        printUsage(argv[0]);
    }
    if (!Utils::isAllDigits(argv[1])) {
        std::cerr << "Error: Port must be a valid number.\n";
        printUsage(argv[0]);
    }
    __attribute__((unused)) SignalHandler signalHandler;
    Server                                server(argv[1], argv[2]);
    try {
        server.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
